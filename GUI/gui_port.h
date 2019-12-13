#ifndef GUI_PORT_H
#define GUI_PORT_H

#include <QGraphicsItem>
#include <QObject>

class GUI_port : public QGraphicsItem
{
public:
    GUI_port(QPointF basePoint, QGraphicsItem *parent);

    void setHoverBoxVisible(bool vis = true);
    void setBasePoint(QPointF basePoint);

    QPointF center();

private:
    QRectF* hoverBox;
    QRectF* portBox;
    bool hoverBoxVisible = false;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool contains(const QPointF &point) const override;

    // QGraphicsItem interface
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

#endif // GUI_PORT_H
