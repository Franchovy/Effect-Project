#include "gui_line.h"

#include <QPainter>

GUI_line::GUI_line(QGraphicsItem* parent) : QGraphicsItem(parent)
{
    p1 = QPointF(0,0);
    p2 = QPointF(0,0);
}

QPointF GUI_line::getP2() const
{
    return p2;
}

void GUI_line::setP2(const QPointF &value)
{
    p2 = value;
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
    painter->setPen(QColor(0,0,0));
    painter->drawLine(p1,p2);
}
