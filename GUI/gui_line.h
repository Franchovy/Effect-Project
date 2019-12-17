#ifndef GUI_LINE_H
#define GUI_LINE_H

#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
class QPen;
QT_END_NAMESPACE

class GUI_line : public QGraphicsItem
{
public:
    GUI_line(QGraphicsItem* parent = nullptr);

    QPointF getP1() const;
    void setP1(const QPointF &value);
    QPointF getP2() const;
    void setP2(const QPointF &value);

    void hoverLeave();

private:
    QPointF p1;
    QPointF p2;

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
