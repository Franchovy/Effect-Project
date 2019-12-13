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

#include "effectsLib/echoeffect1.h"
#include "effectsLib/fuzzeffect.h"
#include "effectsLib/paneffect.h"


#define GRAPHICS_UI


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

void EffectsScene::createEffectUI(Effect* e)
{
    //Create UI for undisplayed Effect e
    if (!e->isUIGenerated()){
#ifdef GRAPHICS_UI
    GUI_effect* e_gui = new GUI_effect(e->effectName);
    e_gui->setPos(QPoint(0,0));
    addItem(e_gui);
    m_effects.append(e_gui);

    e_gui->setAcceptHoverEvents(true);
    //Add Ports to effect
    qDebug() << "Ports: ";
    for (Port* p : e->getPorts()){
        qDebug() << p;
        GUI_port* gp = e_gui->addPort(p,p->getPortType());
        gp->setAcceptHoverEvents(true);

    }
#else
        QGroupBox* w = e->generateUI();
        mainLayout->addWidget(w);
#endif
    }
}

void EffectsScene::deleteEffectUI(Effect *e)
{
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
            if (g->contains(point)){
                return g;
            }
        }
    }
    return nullptr;
}

void EffectsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        draggedItem = itemAt(event->scenePos(), QTransform());
        qDebug() << draggedItem;

        GUI_port* port = getContainingPort(event->scenePos());
        if (port != nullptr){
            qDebug() << "Pressed port";
            portLineDrag = true;
            port_ptr = port;
            portLine = new QLineF(port->center(), port->center());
        } else if (draggedItem != nullptr){
            dragStartPosition = event->scenePos();
            dragRelativePosition = draggedItem->scenePos() - event->scenePos();
            dragging = true;
        }
    }
}
#ifdef mousemove
void EffectsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (dragging){
        draggedItem->setPos(dragRelativePosition + event->scenePos());
    } else if (portLineDrag){
        qDebug() << "Drawing Line to: " << event->scenePos();
        portLine->setP2(event->scenePos());
        QGraphicsScene::mouseMoveEvent(event);
    } else {
        QGraphicsScene::mouseMoveEvent(event);
    }
    update();
}
#endif
void EffectsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (portLineDrag){
        GUI_port* port = getContainingPort(event->scenePos());
        if (port != nullptr){
            //Connected to port!
            port_ptr = port;
            portLine->setP2(port->center());
        } else {
            //Nothing connected so delete temp data.
            port_ptr = nullptr;
            portLine->~QLineF();
            portLine = nullptr;
        }
        portLineDrag = false;
    }

    dragging = false;
    draggedItem = nullptr;
    //dragstart and dragrelative ignored

    update();
}





