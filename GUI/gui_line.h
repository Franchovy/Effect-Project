#ifndef GUI_LINE_H
#define GUI_LINE_H

#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
class QPen;
class GUI_effect;
class GUI_port;
QT_END_NAMESPACE

class GUI_line : public QGraphicsItem
{
public:
    GUI_line(QPointF p1, QPointF p2, QGraphicsItem* parent = nullptr);

    QPointF p1;
    QPointF p2;

private:
    bool hover = false;
    QPen* pen_normal;
    QPen* pen_highlight;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

#endif // GUI_LINE_H
