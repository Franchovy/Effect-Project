#include "effectsscene.h"

#include <QDebug>
#include <QGridLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QDrag>
#include <QKeyEvent>
#include <QMap>
#include <QMimeData>
#include <QStateMachine>
#include <QApplication>

#include "effect.h"
#include "effectmap.h"
#include "ports/port.h"
#include "GUI/gui_effect.h"
#include "GUI/gui_port.h"
#include "GUI/gui_line.h"

#define DELETE_KEY 16777223
#define ESCAPE_KEY 16777216
#define SPACE_KEY 32
#define SHIFT_KEY 16777248
#define CTRL_KEY 16777249

EffectsScene::EffectsScene(QWidget *parent) : QGraphicsScene(parent)
  , mainLayout(new QGridLayout())
  , m_effects(new QSet<Effect*>)
  , m_effectPorts(new QMap<Effect*, QList<GUI_port*>>())
  , m_GUIeffects(new QMap<GUI_effect*, Effect*>())
  , m_GUIports(new QMap<GUI_port*, Effect*>())
  , m_connections(new QMap<QPair<QPair<Effect*,int>,QPair<Effect*,int>>,GUI_line*>())
{
    // Set up state handling

    dragState = neutral;

    //Default settings or whatever
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect(-1000, -1000, 2000, 2000);
    setBackgroundBrush(QColor(255,255,255));

    transform = QTransform();

    selectedItems = QList<GUI_item*>();

    portLine = new GUI_line(QPointF(0,0),QPointF(0,0));
    portLine->setZValue(2);
    addItem(portLine);

    connect(this, &EffectsScene::connectPortsSignal, this, &EffectsScene::connectPorts);
    connect(this, &EffectsScene::deleteEffectSignal, this, &EffectsScene::deleteEffect);
    connect(this, &EffectsScene::disconnectPortsSignal, this, &EffectsScene::disconnectPorts);

    portLine->hide();

    parent->setAcceptDrops(true);
}

void EffectsScene::addEffect(Effect* e)
{
    QPointF pos = QPointF(10*posCount,10*posCount);
    posCount++;
    GUI_effect* e_gui = new GUI_effect(e->effectName, pos);
    m_GUIeffects->insert(e_gui,e);
    m_effects->insert(e);

    qDebug() << "Creating: " << e->effectName;
    addItem(e_gui);
    newEffect = e;

    // GUI_effect position actually set?

    m_effects->insert(e);

    //Add Ports to effect
    int i = 0;

    //counter of type - used for positioning
    QVector<int> portTypeCount = {0,0};

    for (i = 0; i < e->getPorts().size(); i++){
        GUI_port* gp = new GUI_port(e_gui->pos, e->getPorts().at(i)->portType, e_gui);
        portTypeCount.replace(gp->portType,
                              portTypeCount.at(gp->portType)+1); //increment port count for type
        gp->portNumber = i;
        m_GUIports->insert(gp, e);
        QList<GUI_port*> portlist = m_effectPorts->take(e);
        portlist.append(gp);
        m_effectPorts->insert(e, portlist);
        addItem(gp);
    }
    // Position effects
    QVector<int> vcounter = {0,0};
    for (int i = 0; i < e->getPorts().size(); i++){
        GUI_port* p = m_effectPorts->value(e).at(i);
        int count = portTypeCount.at(p->portType);
        int counter = vcounter.at(p->portType);
        // 50 if out, 150 if in
        QPointF dpos = QPointF(150 - 100*p->portType, 100 - 25*(count-1) + 50*counter);
        vcounter.replace(p->portType, counter+1);
        p->setPos(e_gui->pos - dpos);
    }

    // Select new
    select(e_gui);

    // Center view and grab keyboard input
    getView()->centerOn(e_gui->pos);
    //getView()->grabKeyboard();
}

void EffectsScene::deleteEffect(Effect *e)
{
    m_effects->remove(e);
    m_effectPorts->remove(e);
    GUI_effect* e_gui = m_GUIeffects->key(e);
    m_GUIeffects->remove(e_gui);
    removeItem(e_gui);
    deselect(e_gui);
    e_gui->~GUI_effect();
    for (GUI_port* p : m_GUIports->keys(e)){
        GUI_line* l = getConnection(e, p->portNumber);
        if (l) {
            QPair<QPair<Effect*, int>,QPair<Effect*, int>> c = m_connections->key(l);
            disconnectPortsSignal(c.first,c.second);
        }
        m_GUIports->remove(p);
    }
    //e->~Effect();
    update();
}

void EffectsScene::disconnectPorts(QPair<Effect *, int> e1, QPair<Effect *, int> e2)
{
    GUI_line* l = m_connections->take(QPair<QPair<Effect*,int>,QPair<Effect*,int>>(e1,e2));
    if (selectedItems.contains(l)) selectedItems.removeOne(l);
    removeItem(l);
    l->~GUI_line();
}

void EffectsScene::connectPorts(QPair<Effect *, int> e1, QPair<Effect *, int> e2)
{
    GUI_line* l1 = getConnection(e1.first, e1.second);
    GUI_line* l2 = getConnection(e2.first, e2.second);
    if (l1 != nullptr){
        disconnectPorts(m_connections->key(l1).first, m_connections->key(l1).second);
    }
    if (l2 != nullptr){
        disconnectPorts(m_connections->key(l2).first, m_connections->key(l2).second);
    }


    GUI_line* l = new GUI_line(m_effectPorts->value(e1.first).at(e1.second)->pos,
                               m_effectPorts->value(e2.first).at(e2.second)->pos);


    m_connections->insert(QPair<QPair<Effect*,int>,QPair<Effect*,int>>(e1,e2), l);
    addItem(l);
}

void EffectsScene::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == DELETE_KEY){
        // Delete key
        for (GUI_item* item : selectedItems){
            if (item->data(0) == "effect"){
                deleteEffectSignal(m_GUIeffects->value(static_cast<GUI_effect*>(item)));
            } else if (item->data(0) == "line"){
                QPair<QPair<Effect*, int>,QPair<Effect*, int>> pair =
                        m_connections->key(static_cast<GUI_line*>(item));
                disconnectPortsSignal(pair.first, pair.second); //TODO something wrong here?
            } else {
                qDebug() << "Default delete operation on " << item;
                if (item)
                    item->~GUI_item();
            }
        }
    } else if (event->key() == ESCAPE_KEY){
        clearSelectedItems();
    } else if (event->key() == SPACE_KEY){
        // ?
    } else if (event->key() == SHIFT_KEY){
        shiftState = true;
    } else if (event->key() == CTRL_KEY){
        ctrlState = true;
    }
    //qDebug() << event->key();
    update();
}

void EffectsScene::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == SHIFT_KEY){
        shiftState = false;
    } else if (event->key() == CTRL_KEY){
        ctrlState = false;
    }
}

void EffectsScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if (ctrlState){
        qDebug() << event->delta();
        getView()->scale(1+event->delta()*0.001, 1+event->delta()*0.001);
    } else {
        QList<GUI_item*> items = getItemsAt(event->scenePos());
        if (items.length() > 1){
            // Reorder
            for (GUI_item* i : items)
                i->setZValue(-1);
            items.at(scrollRef)->setZValue(1);
            // Change scrollref accordingly
            if (event->delta() > 0){
                scrollRef++;
                if (scrollRef > items.length()){
                    scrollRef = items.length();
                }
            }
            else if (event->delta() < 0){
                scrollRef--;
                if (scrollRef < 0){
                    scrollRef = 0;
                }
            }
        }
    }
    event->accept();
}

void EffectsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        dragDistance = 0;
        clicked_item = hover_item;
        if (clicked_item == nullptr){
            dragState = view;
        } else if (clicked_item->data(0) == "effect"){
            dragState = drag;
        } else if (clicked_item->data(0) == "port"){
            dragState = line;
        }
    } else if (event->button() == Qt::RightButton){

    }
    update();
}

void EffectsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF d = event->scenePos() - event->lastScenePos();

    if(dragState == neutral){
        hover_item = getScrollSelected(getItemsAt(event->scenePos())); // TODO secure scrollRef reference to list (update scrollRef)
    } else if (dragState == view){
        //QPointF v = getView()->pos();
        //getView()->translate(1,1);
    } else if (dragDistance > 10){
        if (dragState == drag){
            dragItem(clicked_item, d);
        } else if (dragState == line){
            dragLine(static_cast<GUI_port*>(clicked_item), event->scenePos());
        }
    } else {
        dragDistance += d.manhattanLength();
    }

    // Send info to qgraphicsscene for hover events.
    if (dragState != view){
        event->setButtons(Qt::MouseButton::NoButton);
        QGraphicsScene::mouseMoveEvent(event);
    }
    update();
}

//TODO encapsulate selection mechanism into gui_item.
void EffectsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (dragState != view && dragDistance < 10){
        // select / deselect
        if (selectedItems.contains(clicked_item)){
            deselect(clicked_item);
        } else {
            select(clicked_item);
        }
    } else if (dragState == line) {
        //dragLine(static_cast<GUI_port*>(clicked_item), event->scenePos());
        connectLine(static_cast<GUI_port*>(clicked_item), event->scenePos());
    } else if (dragState == drag) {
        deselect(clicked_item);
    }
    // Reset state
    dragState = neutral;
    clicked_item = nullptr;
    update();
}

GUI_line *EffectsScene::getConnection(Effect *e, int n)
{
    if (m_connections->isEmpty()) return nullptr;

    for(QPair<QPair<Effect*, int>,QPair<Effect*, int>> pair : m_connections->keys()){
        if ((pair.first.first == e && pair.first.second == n) // If either of pairs match
                || (pair.second.first == e && pair.second.second == n)){
            return m_connections->value(pair);
        }
    }
    return nullptr;
}

void EffectsScene::dragLine(GUI_port* port, QPointF pos)
{
    // If portline has not been initialised
    if (!portLine->isVisible()){
        // Remove existing line if exists
        GUI_line* line = getConnection(m_GUIports->value(port), port->portNumber);
        if (line != nullptr){
            // SEND DISCONNECT SIGNAL
            disconnectPortsSignal(m_connections->key(line).first, m_connections->key(line).second);
        }

        // Create line starting at selected Port center
        portLine->p1 = port->pos;
        portLine->show();
    }
    // Move p2
    portLine->p2 = pos;
}

void EffectsScene::connectLine(GUI_port* port1, QPointF pos)
{
    GUI_port* port2 = nullptr;
    for(GUI_item* item : getItemsAt(pos)){
        if (GUI_port* x = dynamic_cast<GUI_port*>(item)){
            port2 = x;
        }
    }

    if (port1 != nullptr && port2 != nullptr && port1 != port2){
        // Create new line - this is done after signal?
        //GUI_line* newLine = new GUI_line(port1->pos, port2->pos);
        // Connection signal
        connectPortsSignal(
                    QPair<Effect*, int>(m_GUIports->value(port1), port1->portNumber),
                    QPair<Effect*, int>(m_GUIports->value(port2), port2->portNumber));
    }
    portLine->hide();
}

QList<GUI_port *> EffectsScene::getPortsOfType(Effect* e, int type)
{
    QList<GUI_port*> list;
    for (GUI_port* p : m_effectPorts->value(e)){
        if (p->portType == type){
            list.append(p);
        }
    }
    return list;
}

void EffectsScene::dragItem(GUI_item* item, QPointF d)
{
    if (item->data(0) == "effect"){
        // Normal drag - update data as well
        Effect* e = m_GUIeffects->value(static_cast<GUI_effect*>(item));
        m_GUIeffects->key(e)->drag(d);
        for (GUI_port* p : m_GUIports->keys(e)){
            p->pos = p->pos + d;
            GUI_line* l = getConnection(e,p->portNumber);
            if (l != nullptr){
                // Move line
                if (m_connections->key(l).first == QPair<Effect*,int>(e,p->portNumber)){
                    // Move p1
                    l->p1 = l->p1 + d;
                } else if (m_connections->key(l).second == QPair<Effect*,int>(e,p->portNumber)){
                    // Move p2
                    l->p2 = l->p2 + d;
                }
            }
        }
    }
}

void EffectsScene::clearSelectedItems()
{
    for (GUI_item* item : selectedItems){
        deselect(item, true);
    }
}

QList<GUI_item*> EffectsScene::getItemsAt(QPointF pos)
{
    QList<GUI_item*> list = QList<GUI_item*>();
    for (QGraphicsItem* item : items(pos)){
        GUI_item* gitem = static_cast<GUI_item*>(item);
        if (gitem){
            list.append(gitem);
        }
    }
    return list;
}

GUI_item* EffectsScene::getScrollSelected(QList<GUI_item*> list)
{
    if (list.length() <= scrollRef){
        scrollRef = 0;
        return nullptr;
    } else {
        return list.at(scrollRef);
    }
}

QGraphicsView *EffectsScene::getView()
{
    if (views().empty()){
        // View is no longer valid.
        return nullptr;
    } else {
        return graphicsView;
    }
}

void EffectsScene::select(GUI_item *item)
{
    if (!shiftState){
        clearSelectedItems();
    }
    if (item != nullptr && !selectedItems.contains(item)){
        selectedItems.append(item);
        item->select();
    }

}

void EffectsScene::deselect(GUI_item *item, bool clearOp)
{
    if (shiftState || clearOp){
        selectedItems.removeOne(item);
        item->deselect();
    } else {
        if (selectedItems.contains(item) && selectedItems.length() > 1){
            clearSelectedItems();
            select(item);
        } else {
            selectedItems.removeOne(item);
            item->deselect();
        }
    }
}

void EffectsScene::setView(QGraphicsView *value)
{
    graphicsView = value;
}

QString EffectsScene::compileSaveEffect()
{
    //Uses selectedItems list
    QString output = "";
    QList<Effect*> effectsList = QList<Effect*>();

    //Add effects to output
    output.append("EFFECTS:\n");
    for (GUI_item* item : selectedItems){
        if (item->data(0) == "effect"){
            Effect* e = m_GUIeffects->value(static_cast<GUI_effect*>(item));

            output.append(QString::number(effectsList.length()));
            output.append(" : ");
            output.append(QString::number(e->type));
            output.append("\n");

            effectsList.append(e);
        }
    }

    //Add connections between selected effects
    output.append("CONNECTIONS:\n");
    for (QPair<QPair<Effect*, int>, QPair<Effect*, int>> c : m_connections->keys()){
    //for (GUI_line* l : m_connections->values()){
        // Line must be connected to two of selected effects.
        if (effectsList.contains(c.first.first) &&
                effectsList.contains(c.second.first)){
            int port1 = c.first.second;
            int port2 = c.second.second;

            output.append(QString::number(effectsList.indexOf(c.first.first)));
            output.append(" ");
            output.append(QString::number(port1));
            output.append(" : ");
            output.append(QString::number(effectsList.indexOf(c.second.first)));
            output.append(" ");
            output.append(QString::number(port2));
            output.append("\n");
        }
    }
    //output
    return output;
}

bool EffectsScene::loadEffect(QPair<QList<QPair<int, int>>, QList<QPair<QPair<int, int>, QPair<int, int>>>> data)
{
    // data: effects list [id, effect] / connectionslist [id, port / id, port]
    qDebug() << data;
    QVector<Effect*> effects = QVector<Effect*>(data.first.length());
    //Create effects
    for (QPair<int,int> pair : data.first){
        newEffectSignal(pair.second); //operation sets new effect.
        effects[pair.first] = newEffect;
    }
    //QList<QPair<QPair<int, int>, QPair<int, int>>> connections;
    for (QPair<QPair<int,int>,QPair<int,int>> pair : data.second){
        if (m_effectPorts->value(effects[pair.first.first]).at(pair.first.second) != nullptr
                && m_effectPorts->value(effects[pair.second.first]).at(pair.second.second) != nullptr)
        {
            connectPortsSignal(QPair<Effect*, int>(effects[pair.first.first], pair.first.second),
                    QPair<Effect*, int>(effects[pair.second.first], pair.second.second));
        }
    }
    return true;
}
