#ifndef PORT_H
#define PORT_H

#include <QObject>
#include <QPointF>

QT_BEGIN_NAMESPACE
class Effect;
class QComboBox;
class QPushButton;
class GUI_port;
QT_END_NAMESPACE

class Port : public QObject
{
    Q_OBJECT
public:
    explicit Port(QString name, Effect *parent = nullptr);

    QString getName(){return portName;}

    int portType;

    int dataLength = 0;
    char* data;

protected:
    Port* connectedPort = nullptr;
    QString portName;

Q_SIGNALS:
    void constructor(Port* this_ptr);
};

#endif // PORT_H
