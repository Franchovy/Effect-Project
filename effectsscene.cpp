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
    setSceneRect(-200, -200, 400, 400);
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
    qDebug() << "Creating effect: " << e->effectName;
    addItem(e_gui);
    m_effects.append(e_gui);

    //Add Ports to effect
    for (Port* p : e->getPorts()){
        GUI_port* gp = e_gui->addPort(p,p->getPortType());
    }
    return e_gui;
}

void EffectsScene::deleteEffect(Effect *e)
{
    //CHANGEME Delete Effect must be changed
    mainLayout->removeWidget(e->getUI());
    e->getUI()->deleteLater(); //Should not have to do this?
    mainLayout->update();
}


int EffectsScene::getNewEffectType() const
{
    return newEffectType;
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

void EffectsScene::effect_constructor(Effect *ptr)
{
    GUI_effect* e = addEffect(ptr);
}

void EffectsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        draggedItem = itemAt(event->scenePos(), QTransform());

        GUI_port* port = getContainingPort(event->scenePos());
        if (port != nullptr){
            portLineDrag = true;
            port_ptr = port;
            //Create line starting at selected Port center
            portLine = new GUI_line();
            portLine->setP1(port->center());
            portLine->setP2(event->scenePos());
            addItem(portLine);
        } else if (draggedItem != nullptr){
            dragging = true;
        }
    }
    update();
}

void EffectsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (dragging){
        QPointF d = event->scenePos() - event->lastScenePos();
        draggedItem->moveBy(d.x(),d.y());
    } else if (portLineDrag){
        portLine->setP2(event->scenePos());
        QGraphicsScene::mouseMoveEvent(event);
    } else {
        QGraphicsScene::mouseMoveEvent(event);
    }
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
            connectPorts(port->getPort(), port_ptr->getPort());
        } else {
            //Nothing connected so delete temp data.
            //CHANGEME no garbage disposal implemented
            port_ptr = nullptr;
            portLine->~GUI_line();
            portLine = nullptr;
        }
        portLineDrag = false;
    }

    dragging = false;
    draggedItem = nullptr;

    update();
}





