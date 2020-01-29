#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include "gui_item.h"

#include <QGraphicsScene>



class gui_interface : public GUI_item
{
public:
    gui_interface(QGraphicsScene* parent = nullptr);

private:
    QRectF selectionRectangle;


    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // GUI_INTERFACE_H
