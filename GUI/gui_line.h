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
    GUI_line(GUI_port* p1 = nullptr, GUI_port* p2 = nullptr, QGraphicsItem* parent = nullptr);

    QPointF getP1() const;
    void setP1(const QPointF &value);
    QPointF getP2() const;
    void setP2(const QPointF &value);

    GUI_port *getPort1() const;
    void setPort1(GUI_port *value);

    GUI_port *getPort2() const;
    void setPort2(GUI_port *value);

    void connectPorts();

    GUI_port* getPortFromEffect(GUI_effect* e) const;

    void drag(GUI_port *port, QPointF point);

    void destroy();

    void hoverLeave();

private:
    GUI_port* port1 = nullptr;
    GUI_port* port2 = nullptr;
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
