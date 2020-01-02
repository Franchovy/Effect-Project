#include "gui_port.h"

#include <QPainter>
#include <QDebug>

#include "gui_effect.h"
#include "gui_line.h"
#include "ports/port.h"

GUI_port::GUI_port(QPointF basePoint, int portType, GUI_effect* parent) : GUI_item(parent)
{
    setData(0,"port");
    this->portType = portType;
    pos = basePoint;

    hoverBox = new QRectF(QPointF(0,0), QSize(60,60));
    portBox = new QRectF(QPointF(0,0), QSize(20,20));
    hoverBox->moveCenter(basePoint);
    portBox->moveCenter(basePoint);

    portNumber = 0;

    setAcceptHoverEvents(true);
}

void GUI_port::setHoverBoxVisible(bool vis)
{
    hoverBoxVisible = vis;
    update();
}

QRectF GUI_port::boundingRect() const
{
    return *hoverBox;
}

void GUI_port::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor color_blue = QColor(100,100,255,150);
    QColor color_black = QColor(0,0,0);
    QPen pen = QPen(color_blue, 2);
    painter->setPen(pen);

    if (hoverBoxVisible){
        painter->drawRect(*hoverBox);
    }
    pen.setColor(color_black);
    painter->setPen(pen);
    painter->drawRect(*portBox);
}

bool GUI_port::contains(const QPointF &point) const
{
    return hoverBox->contains(point);
}

void GUI_port::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setHoverBoxVisible(true);
}

void GUI_port::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setHoverBoxVisible(false);
}




