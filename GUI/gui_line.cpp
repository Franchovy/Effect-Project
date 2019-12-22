#include "gui_line.h"

#include "gui_port.h"
#include "gui_effect.h"

#include <QDebug>
#include <QPainter>
#include <QPen>

GUI_line::GUI_line(GUI_port* p1, GUI_port* p2, QGraphicsItem* parent) : QGraphicsItem(parent)
{
    setData(0,"line");

    //Assign points
    port1 = p1;
    port2 = p2;
    if (port1 != nullptr) {this->p1 = port1->center();}
    if (port2 != nullptr) {this->p2 = port2->center();}


    setAcceptHoverEvents(true);

    pen_normal = new QPen(QColor(0,0,0));
    pen_highlight = new QPen(QColor(100,100,255,150),2);
}

QPointF GUI_line::getP2() const
{
    return p2;
}

void GUI_line::setP2(const QPointF &value)
{
    p2 = value;
}

void GUI_line::hoverLeave()
{
    hover = false;
}

GUI_port *GUI_line::getPort2() const
{
    return port2;
}

void GUI_line::setPort2(GUI_port *value)
{
    port2 = value;
    connectPorts();
}

GUI_port *GUI_line::getPortFromEffect(GUI_effect *e) const
{

    if (port1 == nullptr) return nullptr;
    if (port2 == nullptr) return nullptr;

    if (e->getPorts().contains(port1)){
        return port1;
    } else if (e->getPorts().contains(port2)){
        return port2;
    }
    return nullptr;
}

void GUI_line::drag(GUI_port *port, QPointF point)
{
    if (port == port1){
        setP1(p1 + point);
    } else if (port == port2){
        setP2(p2 + point);
    }
}

void GUI_line::destroy()
{
    //port1->eraseconnection
    //port2->eraseconnection
    this->~GUI_line();
}

GUI_port *GUI_line::getPort1() const
{
    return port1;
}

void GUI_line::setPort1(GUI_port *value)
{
    port1 = value;
    connectPorts();
}

QPointF GUI_line::getP1() const
{
    return p1;
}

void GUI_line::setP1(const QPointF &value)
{
    p1 = value;
}

void GUI_line::connectPorts()
{
    if (port1 != nullptr && port2 != nullptr){
        port2->connectPort(port1->getPort(), this);
        port1->connectPort(port2->getPort(), this);
    }
}

QRectF GUI_line::boundingRect() const
{
    return QRectF(p1, p2);
}

void GUI_line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (hover){
        painter->setPen(*pen_highlight);
    } else {
        painter->setPen(*pen_normal);
    }

    painter->drawLine(p1,p2);
}

void GUI_line::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    hover = true;
}

void GUI_line::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    hover = false;
}
