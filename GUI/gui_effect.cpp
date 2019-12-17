#include "gui_effect.h"

#include <QDebug>
#include <QPainter>
#include <QGraphicsScene>

#include "gui_port.h"
#include "ports/port.h"

GUI_effect::GUI_effect(QString name, QGraphicsItem* parent) : QGraphicsItem(parent)
  , m_inPorts(QList<GUI_port*>())
  , m_outPorts(QList<GUI_port*>())
{
    setData(0,"effect");
    baseRect = QRectF(-200,-200,200,200);
    title = name;

    setAcceptHoverEvents(true);

    pen_normal = new QPen(QColor(0,0,0));
    pen_highlight = new QPen(QColor(100,100,255,150),2);
}

GUI_port* GUI_effect::addPort(Port *port, int portType)
{
    int dy;
    int dx;

    //TODO worth adding a check of port type.

    QPointF* point = new QPointF();
    GUI_port* new_port = new GUI_port(*point, port, this);

    //Add port to GUI
    switch (portType){
    case 1:
        //InPort
        m_inPorts.append(new_port);
        dy = 30 + 30 * m_inPorts.length();
        dx = 30;
        break;
    case 2:
        //OutPort
        m_outPorts.append(new_port);
        dy = 30 + 30 * m_outPorts.length();
        dx = 150;
        break;
    default:
        //unregistered type
        dx = 10;
        dy = 10;
    }

    point->setX(boundingRect().x()+dx);
    point->setY(boundingRect().y()+dy);
    new_port->setBasePoint(*point);
    return new_port;
}

QList<GUI_port*> GUI_effect::addPort(QList<Port*> portList, int portType)
{
    QList<GUI_port*> list = QList<GUI_port*>();
    for (Port* p : portList){
        list.append(addPort(p, portType));
    }
    return list;
}

QList<GUI_port *> GUI_effect::getPorts()
{
    QList<GUI_port*> list = QList<GUI_port*>();
    list.append(m_inPorts);
    list.append(m_outPorts);
    return list;
}


QRectF GUI_effect::boundingRect() const
{
    return baseRect;
}

void GUI_effect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QColor(0,0,0));

    //Set up title properly with boundingrect and font
    QFont font = painter->font();
    font.setPixelSize(20);
    painter->setFont(font);
    
    if (hover){
        painter->setPen(*pen_highlight);
        painter->drawRect(QRectF(baseRect.topLeft()-QPoint(5,5), baseRect.bottomRight()+QPoint(5,5)));
    }

    painter->setPen(*pen_normal);
    painter->drawRect(baseRect);
    painter->drawText(QPointF(baseRect.topLeft().x()+10, baseRect.topLeft().y()+10), title);
}

void GUI_effect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    hover = true;
}

void GUI_effect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    hover = false;
}

