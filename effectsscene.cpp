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
#include <QMap>
#include <QMimeData>
#include <QApplication>

#include "effect.h"
#include "effectmap.h"
#include "ports/port.h"
#include "GUI/gui_effect.h"
#include "GUI/gui_port.h"
#include "GUI/gui_line.h"



EffectsScene::EffectsScene(QWidget *parent) : QGraphicsScene(parent)
    , mainLayout(new QGridLayout())
{
    m_effectPorts = new QMap<Effect*, QPointF[]>();
    m_effects = new QMap<Effect*, QPointF>();

    //Default settings or whatever
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect(-1000, -1000, 2000, 2000);
    setBackgroundBrush(QColor(255,255,255));

    deviceTransform = QTransform();

    parent->setAcceptDrops(true);
}



void EffectsScene::addEffect(Effect* e)
{
    QPointF pos = newEffectPos(e);
    GUI_effect* e_gui = new GUI_effect(e->effectName, pos);
    m_effects->insert(e, pos);
    addItem(e_gui);

    // GUI_effect position actually set?

    m_effects->insert(e,pos);

    //Add Ports to effect
    int i = 0;
    while (!m_effectPorts->find(e).value()[i].isNull()){ //WARNING this ok? ->isNull() for an array? / fault to occur
        GUI_port* gp = new GUI_port(m_effectPorts->find(e).value()[i]);
        addItem(gp);
    }
}

void EffectsScene::deleteEffect(Effect *e)
{
    //TODO Delete Effect
}

void EffectsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        draggedItem = itemAt(event->scenePos(), QTransform());

        if (draggedItem->data(0) == "line"){
            //Save line state
            portLine = static_cast<GUI_line*>(draggedItem);
            portLine->hide();
            splitLineCreate(event->scenePos());
            dragging = true;

        } else if (draggedItem->data(0) == "port"){
            portLineDrag = true;
            //Create line starting at selected Port center
            portLine->p1 = getPortCenter(event->scenePos());
            portLine->p2 = event->scenePos();
            portLine->show();

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
    QPointF d = event->scenePos() - event->lastScenePos();

    if (dragging){
        if (draggedItem->data(0) == "line"){
            splitLineDrag(event->scenePos());
        } else if (draggedItem->data(0) == "effect"){
            GUI_effect* e = static_cast<GUI_effect*>(draggedItem);
            e->drag(d);
        } else {
            draggedItem->moveBy(d.x(),d.y());
        }
    } else if (portLineDrag){
        portLine->p2 = event->scenePos();
    } else if (dragView){
        //TODO drag view
    }
    QGraphicsScene::mouseMoveEvent(event);

    update();
}

void EffectsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (portLineDrag){
        if (getPortCenter(event->scenePos()).isNull()){
            connectLine();
        } else {
            //Nothing connected so delete temp data.
            portLine->hide();
        }
        portLineDrag = false;
    } else if (dragging){
        if (draggedItem->data(0) == "line"){
            //Unsuccessful line split
            splitLineErase();
            portLine->show();
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

QPointF EffectsScene::newEffectPos(Effect *)
{

}

QList<GUI_port *> EffectsScene::getPorts(Effect *)
{

}

Effect *EffectsScene::getEffectAt(QPointF)
{

}

QPointF EffectsScene::getPortCenter(QPointF)
{

}

void EffectsScene::connectLine()
{
    // Check if portLine has both valid ports
    // if true
    //     Create new (permanent) line
         portLine->hide();
    //    connectPortsSignal();
    // else
    //    hide portLine, leave nothing
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


void EffectsScene::splitLineCreate(QPointF p)
{
    //Separate out line into two
    portLines.first = new GUI_line(p, portLine->p1);
    portLines.second = new GUI_line(portLine->p2, p);

    addItem(portLines.first);
    addItem(portLines.second);
}

void EffectsScene::splitLineDrag(QPointF p)
{
    portLines.first->p2 = p;
    portLines.second->p1 = p;
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

GUI_line *EffectsScene::connectPorts(QPair<Effect*, int> e1, QPair<Effect*, int> e2)
{
    GUI_line* line;

    line = new GUI_line(
                m_effectPorts->find(e1.first).value()[e1.second],
                m_effectPorts->find(e2.first).value()[e2.second]
            );
    addItem(line);
    m_connections->insert(QPair<QPair<Effect*,int>,QPair<Effect*,int>>(e1,e2),line);

    return line;
}



