#ifndef EFFECTGUI_H
#define EFFECTGUI_H

#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
class Port;
class GUI_port;
class QGraphicsScene;
QT_END_NAMESPACE

class GUI_effect : public QGraphicsItem
{
public:
    GUI_effect(QString name = "Default name", QGraphicsItem* parent = nullptr);

    GUI_port* addPort(Port* port, int portType);
    QList<GUI_port*> addPort(QList<Port*> portList, int portType);

    QList<GUI_port*> getPorts();

private:
    QRectF baseRect;
    QString title;

    QList<GUI_port*> m_inPorts;
    QList<GUI_port*> m_outPorts;

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

#endif // EFFECTGUI_H
