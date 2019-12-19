#include "gui_line.h"

#include "gui_port.h"

#include <QPainter>
#include <QPen>

GUI_line::GUI_line(GUI_port* p1, GUI_port* p2, QGraphicsItem* parent) : QGraphicsItem(parent)
{
    setData(0,"line");

    //Assign points
    this->p1 = p1->center();
    this->p2 = p2->center();
    port1 = p1;
    port2 = p2;

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
}

GUI_port *GUI_line::getPort1() const
{
    return port1;
}

void GUI_line::setPort1(GUI_port *value)
{
    port1 = value;
}

QPointF GUI_line::getP1() const
{
    return p1;
}

void GUI_line::setP1(const QPointF &value)
{
    p1 = value;
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
