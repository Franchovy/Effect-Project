#ifndef EFFECTGUI_H
#define EFFECTGUI_H

//#include <QObject>
#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
class Port;
class GUI_port;
class QGraphicsScene;
QT_END_NAMESPACE

class GUI_effect : public QGraphicsItem
{
    //Q_OBJECT
public:
    GUI_effect(QString name = "Default name", QGraphicsItem* parent = nullptr);

    GUI_port* addPort(Port* port, int portType);
    QList<GUI_port*> addPort(QList<Port*> portList, int portType);

    QList<GUI_port*> getPorts();

private:
    QRectF baseRect;
    QString title;

    QList<GUI_port*> InPorts;
    QList<GUI_port*> OutPorts;

    QGraphicsScene* parentScene;


    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif // EFFECTGUI_H
