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



EffectsScene::EffectsScene(QWidget *parent) : QGraphicsScene(parent)
  , mainLayout(new QGridLayout())
  , m_effects(new QSet<Effect*>)
  , m_effectPorts(new QMap<Effect*, QList<QPointF>>())
  , m_GUIeffects(new QMap<GUI_effect*, Effect*>())
  , m_GUIports(new QMap<GUI_port*, Effect*>())
  , m_connections(new QMap<QPair<QPair<Effect*,int>,QPair<Effect*,int>>,GUI_line*>())
{
    // Set up state handling

    mouseState = neutral;

    //Default settings or whatever
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect(-1000, -1000, 2000, 2000);
    setBackgroundBrush(QColor(255,255,255));

    transform = QTransform();

    selectedItems = new QList<QGraphicsItem*>();

    portLine = new GUI_line(QPointF(0,0),QPointF(0,0));
    addItem(portLine);

    connect(this, &EffectsScene::connectPortsSignal, this, &EffectsScene::connectPorts);
    connect(this, &EffectsScene::deleteEffectSignal, this, &EffectsScene::deleteEffect);
    connect(this, &EffectsScene::disconnectPortsSignal, this, &EffectsScene::disconnectPorts);

    parent->setAcceptDrops(true);
}

void EffectsScene::addEffect(Effect* e)
{
    QPointF pos = newEffectPos(e);
    GUI_effect* e_gui = new GUI_effect(e->effectName, pos);
    m_GUIeffects->insert(e_gui,e);
    m_effects->insert(e);

    addItem(e_gui);

    // GUI_effect position actually set?

    m_effects->insert(e);
    m_effectPorts->insert(e, e->getPortLocs());


    //Add Ports to effect
    int i = 0;
    if (m_effectPorts->contains(e)){
        for (i = 0; i < m_effectPorts->find(e).value().size(); i++){
            GUI_port* gp = new GUI_port(e_gui->pos - m_effectPorts->find(e).value().at(i), e->getPorts().at(i)->portType, e_gui);
            gp->portNumber = i;
            m_GUIports->insert(gp, e);
            addItem(gp);
        }
    }
}

void EffectsScene::deleteEffect(Effect *e)
{
    m_effects->remove(e);
    m_effectPorts->remove(e);
    GUI_effect* g = m_GUIeffects->key(e);
    m_GUIeffects->remove(g);
    removeItem(g);
    g->~GUI_effect();
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
    if (selectedItems->contains(l)) selectedItems->removeOne(l);
    removeItem(l);
    l->~GUI_line();
}

void EffectsScene::connectPorts(QPair<Effect *, int> e1, QPair<Effect *, int> e2)
{
    GUI_line* l = new GUI_line(m_GUIeffects->key(e1.first)->pos + m_effectPorts->value(e1.first).at(e1.second),
                               m_GUIeffects->key(e2.first)->pos + m_effectPorts->value(e2.first).at(e2.second));

    //GUI_line* l = new GUI_line(portLine->p1,portLine->p2,portLine->parentItem());
    m_connections->insert(QPair<QPair<Effect*,int>,QPair<Effect*,int>>(e1,e2), l);
    addItem(l);

}

void EffectsScene::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == 16777223){
        for (QGraphicsItem* item : *selectedItems){
            if (item->data(0) == "effect"){
                deleteEffectSignal(m_GUIeffects->value(static_cast<GUI_effect*>(item)));
            } else if (item->data(0) == "line"){
                QPair<QPair<Effect*, int>,QPair<Effect*, int>> pair =
                        m_connections->key(static_cast<GUI_line*>(item));
                disconnectPortsSignal(pair.first, pair.second);
            } else {
                item->~QGraphicsItem();
            }
        }
    }
    update();
    qDebug() << event->key();
}

void EffectsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        dragDistance = 0;
        for (QGraphicsItem* item : items(event->scenePos())){
            if (selectedItems->contains(item)){
                mouseState = deselecting;
            } else {
                selectedItems->append(item);
                mouseState = selecting;
            }
            break;
        }
    }
    update();
}

void EffectsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsView* v = getView();
    QPointF d = event->scenePos() - event->lastScenePos();

    if(mouseState == selecting || mouseState == deselecting){
        if (dragDistance > 10){
            if (selectedItems->first()->data(0) == "line"){ // Top selected item is line
                //Save line state
                portLine = static_cast<GUI_line*>(selectedItems->first());
                portLine->hide();
                splitLineStart(event->scenePos());

                mouseState = splitlines;
            } else if (selectedItems->first()->data(0) == "port"){
                GUI_port* p = static_cast<GUI_port*>(selectedItems->first());

                // Remove existing lines
                int n = getPortNumber(event->scenePos());
                GUI_line* line = getConnection(m_GUIports->value(p), n);
                if (line != nullptr){
                    // SEND DISCONNECT SIGNAL

                }

                // Create line starting at selected Port center
                portLine->p1 = p->pos;// + m_GUIeffects->key(m_GUIports->value(p))->pos;
                //portLine->p1 = getPortCenter(event->scenePos());
                portLine->p2 = event->scenePos();
                portLine->show();

                mouseState = linedrag;
            } else {
                mouseState = dragging;
            }
        } else {
            dragDistance += d.manhattanLength();
        }
    }
    if(mouseState == dragging){
        drag(d);
    } else if (mouseState == linedrag){
        portLine->p2 = event->scenePos();
    } else if (mouseState == splitlines){
        splitLineDrag(event->scenePos());
    }

    event->setButtons(Qt::MouseButton::NoButton);
    QGraphicsScene::mouseMoveEvent(event);
    update();
}

//TODO encapsulate selection mechanism into gui_item.
void EffectsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (mouseState == neutral){

    } else if (mouseState == selecting){
        for (QGraphicsItem* item : items(event->scenePos())){
            GUI_item* g = static_cast<GUI_item*>(item);
            if (g) g->select();
        }
    } else if (mouseState == deselecting){
        for (QGraphicsItem* item : items(event->scenePos())){
            if (selectedItems->contains(item)){
                selectedItems->removeOne(item);
                GUI_item* g = static_cast<GUI_item*>(item);
                if (g) g->deselect();
            }
        }
    } else if (mouseState == dragging){
        for (QGraphicsItem* item : *selectedItems){
            GUI_item* g = static_cast<GUI_item*>(item);
            qDebug() << "Item: " << item;
            if (g) g->deselect();
        }
        selectedItems->clear();
    } else if (mouseState == splitlines){
        connectSplitLines();
        QGraphicsScene::mouseReleaseEvent(event);
        selectedItems->clear();
    } else if (mouseState == linedrag){
        connectLine();
        selectedItems->clear();
    }
    mouseState = neutral;
    update();
}

QPointF EffectsScene::newEffectPos(Effect *)
{
    QPointF pos;
    while(itemAt(pos,transform) != nullptr){
        pos = QPointF(pos.x()+10,pos.y());
    }
    return pos;
}

QList<GUI_port *> EffectsScene::getPorts(Effect *e)
{
    QList<GUI_port*>* portList = new QList<GUI_port*>();
    for (GUI_port* p : m_GUIports->keys(e)){
        portList->append(p);
    }
    return *portList;
}

Effect *EffectsScene::getEffectAt(QPointF pos)
{
    for (QGraphicsItem* item : items(pos)){
        if (static_cast<GUI_effect*>(item)->contains(pos)){
            // For now, just return the first one.
            return m_GUIeffects->value(static_cast<GUI_effect*>(item));
        }
    }
}

QPointF EffectsScene::getPortCenter(QPointF pos)
{
    for (QGraphicsItem* item : items(pos)){
        if (static_cast<GUI_port*>(item)->contains(pos)){
            Effect* e = m_GUIports->value(static_cast<GUI_port*>(item));
            int i = static_cast<GUI_port*>(item)->portNumber;
            return m_GUIeffects->key(e)->boundingRect().center();
        }
    }
}

GUI_port *EffectsScene::getPortAt(QPointF pos)
{
    for (QGraphicsItem* item : items(pos)){
        if (Effect* e = m_GUIports->value(static_cast<GUI_port*>(item))){
            return m_GUIports->key(e);
        }
    }
    return nullptr;
}

int EffectsScene::getPortNumber(QPointF pos)
{
    for (QGraphicsItem* item : items(pos)){
        if (item->data(0) == "port"){
            return static_cast<GUI_port*>(item)->portNumber;
        }
    }
    return -1;
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

void EffectsScene::connectLine()
{
    QPair<Effect*,Effect*> effects;
    QPair<int, int> portNumbers;
    for (QGraphicsItem* item : items(portLine->p1)){
        if (item->data(0) == "port"){
            portLine->p1 = static_cast<GUI_port*>(item)->pos;
            qDebug() << static_cast<GUI_port*>(item)->pos;
            portNumbers.first = static_cast<GUI_port*>(item)->portNumber;
            effects.first = m_GUIports->value(static_cast<GUI_port*>(item));
            break;
        } else {
            portLine->p1 = QPointF();
        }
    }
    for (QGraphicsItem* item : items(portLine->p2)){
        if (item->data(0) == "port"){
            portLine->p2 = static_cast<GUI_port*>(item)->pos;
            portNumbers.second = static_cast<GUI_port*>(item)->portNumber;
            effects.second = m_GUIports->value(static_cast<GUI_port*>(item));
            break;
        } else {
            portLine->p2 = QPointF();
        }
    }
    if (effects.first != nullptr && effects.second != nullptr){
        // Successful connection

        connectPortsSignal(QPair<Effect*, int>(effects.first, portNumbers.first),
                           QPair<Effect*, int>(effects.second, portNumbers.second));



        }
    portLine->hide();
}

void EffectsScene::drag(QPointF d)
{
    //TODO effect when dragged seems to be added twice to selectedItems. (issue previously solved by using set).
    for (QGraphicsItem* item : *selectedItems){
        qDebug() << item->data(0);
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
}

QGraphicsView *EffectsScene::getView()
{
    if (views().empty()){
        // View is no longer valid.
        return nullptr;
    } else {
        return view;
    }
}

void EffectsScene::splitLineStart(QPointF p)
{
    //Separate out line into two
    portLines.first = new GUI_line(portLine->p1, p);
    portLines.second = new GUI_line(p, portLine->p2);

    addItem(portLines.first);
    addItem(portLines.second);
}

void EffectsScene::splitLineDrag(QPointF p)
{
    portLines.first->p2 = p;
    portLines.second->p1 = p;
}

void EffectsScene::connectSplitLines()
{
    //TODO properly connect line to ports. (through signal??)

    bool connectionValid = false;
    // If valid middle point
    for (QGraphicsItem* item : items(portLines.first->p2)){
        GUI_port* middle_port = static_cast<GUI_port*>(item);
        if (middle_port != nullptr && item->data(0) == "port"){
            GUI_port* first_port = getPortAt(portLines.first->p1);
            GUI_port* last_port = getPortAt(portLines.second->p2);

            Effect* e1 = m_GUIports->value(first_port);
            Effect* e2 = m_GUIports->value(middle_port);
            Effect* e3 = m_GUIports->value(last_port);

            for (GUI_port* p : m_GUIports->keys(e2)){
                if (p->portType != middle_port->portType){

                    // Emit first connection
                    emit(connectPortsSignal(QPair<Effect*,int>(e1,first_port->portNumber),
                                            QPair<Effect*,int>(e2,middle_port->portNumber)));

                    // Emit second connection (with first free port)
                    emit(connectPortsSignal(QPair<Effect*,int>(e2,p->portNumber),
                                            QPair<Effect*,int>(e3,last_port->portNumber)));
                    connectionValid = true;
                }
            }
        }
    }
    if (!connectionValid){
        portLine->show();
        portLines.first->~GUI_line();
        portLines.second->~GUI_line();
    }
    update();
}

void EffectsScene::splitLineErase()
{
    removeItem(portLines.first);
    removeItem(portLines.second);
    portLines.first->~GUI_line();
    portLines.first = nullptr;
    portLines.second->~GUI_line();
    portLines.second = nullptr;
    addItem(portLine);
}

void EffectsScene::setView(QGraphicsView *value)
{
    view = value;
}

/*
// For internal use or from MainWindow
GUI_line *EffectsScene::connectPorts(QPair<Effect*, int> e1, QPair<Effect*, int> e2)
{
    GUI_line* line = nullptr;


    QList<QPointF> list1 = m_effectPorts->value(e1.first);
    QList<QPointF> list2 = m_effectPorts->value(e2.first);

    line = new GUI_line(m_GUIeffects->key(e1.first)->pos + list1.at(e1.second),
                        m_GUIeffects->key(e2.first)->pos + list2.at(e2.second));


    addItem(line);
    m_connections->insert(QPair<QPair<Effect*,int>,QPair<Effect*,int>>(e1,e2),line);
    if (!list1.empty() && !list2.empty()){
        line = new GUI_line(
                    list1.at(e1.second) + m_GUIeffects->key(e1.first)->pos,
                    list2.at(e2.second) + m_GUIeffects->key(e2.first)->pos
                );

    }

    return line;
}*/
