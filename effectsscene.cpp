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
#include <QMimeData>
#include <QApplication>

#include "effect.h"
#include "effectmap.h"
#include "ports/port.h"
#include "GUI/gui_effect.h"
#include "GUI/gui_port.h"
#include "GUI/gui_line.h"

#include "effectsLib/echoeffect1.h"
#include "effectsLib/fuzzeffect.h"
#include "effectsLib/paneffect.h"


EffectsScene::EffectsScene(QWidget *parent) : QGraphicsScene(parent)
    , mainLayout(new QGridLayout())
    , m_effects(QList<GUI_effect*>())
{
    //Default settings or whatever
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect(-1000, -1000, 2000, 2000);
    setBackgroundBrush(QColor(255,255,255));

    deviceTransform = QTransform();

    parent->setAcceptDrops(true);


}

void EffectsScene::setupEffectsSelect(QComboBox* effectsSelect)
{
    effectTypeList.append("Input");
    effectTypeList.append("Output");
    effectTypeList.append("Echo");
    effectTypeList.append("Pan");
    effectTypeList.append("Fuzz");
    effectsSelect->insertItems(0, effectTypeList);

    effectsSelect->setEditText("Select Effect");

    connect(effectsSelect,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int i){
        newEffectType = i;
    });
}

GUI_effect* EffectsScene::addEffect(Effect* e)
{
    GUI_effect* e_gui = new GUI_effect(e->effectName);
    addItem(e_gui);
    e_gui->setPos(QPointF(-100,0));

    while (!e_gui->collidingItems().empty()){
        //Colliding with other stuff..
        e_gui->moveBy(150,15);
    }

    m_effects.append(e_gui);

    //Add Ports to effect
    for (Port* p : e->getPorts()){
        GUI_port* gp = e_gui->addPort(p,p->getPortType());
        p->setUi(gp);
    }
    return e_gui;
}

void EffectsScene::deleteEffect(Effect *e)
{
    //TODO Delete Effect
}


int EffectsScene::getNewEffectType() const
{
    return newEffectType;
}

void EffectsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        draggedItem = itemAt(event->scenePos(), QTransform());

        if (draggedItem->data(0) == "line"){
            //Save line state
            portLine = static_cast<GUI_line*>(draggedItem);
            removeItem(portLine);
            //Separate out line into two
            portLines.first = new GUI_line();
            portLines.second = new GUI_line();


            portLines.first->setP1(portLine->getP1());
            portLines.second->setP2(portLine->getP2());
            portLines.first->setP2(event->scenePos());
            portLines.second->setP1(event->scenePos());

            addItem(portLines.first);
            addItem(portLines.second);

            dragging = true;

        } else if (draggedItem->data(0) == "port"){
            GUI_port* port = static_cast<GUI_port*>(draggedItem);

            portLineDrag = true;
            port_ptr = port;
            //Create line starting at selected Port center
            portLine = new GUI_line();
            portLine->setP1(port->center());
            portLine->setP2(event->scenePos());
            addItem(portLine);
        } else if (draggedItem->data(0) == "effect"){
            dragging = true;
        }  else {
            dragView = true;
        }
    }
    update();
}

void EffectsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsView* v = getView();
    if (dragging){
        if (draggedItem->data(0) == "line"){
            portLines.first->setP2(event->scenePos());
            portLines.second->setP1(event->scenePos());
        } else {
            QPointF d = event->scenePos() - event->lastScenePos();
            draggedItem->moveBy(d.x(),d.y());
        }
    } else if (portLineDrag){
        portLine->setP2(event->scenePos());
    } else if (dragView){
        //CHANGEME does not work
        QPointF p = event->scenePos() - event->lastScenePos();
        //v==nullptr ? qDebug() << 1 : qDebug() << 2;
        //v->translate(p.rx(),p.ry());
    }
    QGraphicsScene::mouseMoveEvent(event);

    update();
}

void EffectsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (portLineDrag){
        GUI_port* port = getContainingPort(event->scenePos());
        if (port != nullptr){
            //Connected to port!
            portLine->setP2(port->center());
            //Emit connectPort signal
            connectPortsSignal(port->getPort(), port_ptr->getPort());
        } else {
            //Nothing connected so delete temp data.
            //CHANGEME no garbage disposal implemented
            port_ptr = nullptr;
            portLine->~GUI_line();
            portLine = nullptr;
        }
        portLineDrag = false;
    } else if (dragging){
        if (draggedItem->data(0) == "line"){
            //Unsuccessful line split
            removeItem(portLines.first);
            removeItem(portLines.second);
            portLines.first = nullptr;
            portLines.second = nullptr;
            addItem(portLine);
            portLine->hoverLeave();
        } else if (draggedItem->data(0) == "effect"){

        }
    } else if (dragView){
        dragView = false;
    }

    QGraphicsScene::mouseReleaseEvent(event);
    dragging = false;
    draggedItem = nullptr;

    update();
}


QGraphicsView *EffectsScene::getView()
{
    if (views().empty()){
        // View is no longer valid.
        qDebug() << "No view in sight!";
        return nullptr;
    } else {
        return view;
    }
}

GUI_port* EffectsScene::getContainingPort(QPointF point){
    for (GUI_effect* e : m_effects){
        for (GUI_port* g : e->getPorts()){
            if (g->contains(point - e->scenePos())){
                return g;
            }
        }
    }
    return nullptr;
}

void EffectsScene::setView(QGraphicsView *value)
{
    view = value;
}

void EffectsScene::effect_constructor(Effect *ptr)
{
    GUI_effect* e = addEffect(ptr);
}


GUI_line *EffectsScene::connectPorts(Port *p1, Port *p2)
{
    GUI_line* line = new GUI_line(p1->getUI(), p2->getUI());
    addItem(line);
    return line;
}





