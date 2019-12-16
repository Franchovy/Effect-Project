#ifndef GUI_LINE_H
#define GUI_LINE_H

#include <QGraphicsItem>

class GUI_line : public QGraphicsItem
{
public:
    GUI_line(QGraphicsItem* parent = nullptr);

    QPointF getP1() const;
    void setP1(const QPointF &value);
    QPointF getP2() const;
    void setP2(const QPointF &value);

private:
    QPointF p1;
    QPointF p2;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif // GUI_LINE_H
