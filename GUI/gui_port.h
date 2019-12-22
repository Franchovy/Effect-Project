#ifndef GUI_PORT_H
#define GUI_PORT_H

#include <QGraphicsItem>
#include <QObject>

QT_BEGIN_NAMESPACE
class GUI_effect;
class GUI_line;
class Port;
QT_END_NAMESPACE

class GUI_port : public QGraphicsItem
{
public:
    GUI_port(QPointF basePoint);

    void setHoverBoxVisible(bool vis = true);
    void setBasePoint(QPointF basePoint);

    QPointF pos;

private:
    QRectF* hoverBox;
    QRectF* portBox;
    bool hoverBoxVisible = false;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    bool contains(const QPointF &point) const override;

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

};

#endif // GUI_PORT_H
