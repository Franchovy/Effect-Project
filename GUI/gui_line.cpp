#include "gui_line.h"

#include "gui_port.h"
#include "gui_effect.h"

#include <QDebug>
#include <QPainter>
#include <QPen>


GUI_line::GUI_line(QPointF p1, QPointF p2, QGraphicsItem* parent) : GUI_item(parent)
{
    setData(0,"line");

    //Assign points
    this->p1 = p1;
    this->p2 = p2;

    //painter = new QPainter();
    pen_normal = new QPen(QColor(0,0,0));
    pen_highlight = new QPen(QColor(100,100,255,150),2);

    selected = false;

}

QRectF GUI_line::boundingRect() const
{
    return QRectF(p1, p2);
}

void GUI_line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (hover){
        pen_highlight->setStyle(Qt::DashLine);
        painter->setPen(*pen_highlight);
    } else if (selected){
        pen_highlight->setStyle(Qt::SolidLine);
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
