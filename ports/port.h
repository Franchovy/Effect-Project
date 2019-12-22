#ifndef PORT_H
#define PORT_H

#include <QObject>

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

    virtual char* getData();

    QString getName(){return portName;}

    virtual int getConnectPortType(){return 0;}
    virtual int getPortType() {return 0;}

protected:
    Port* connectedPort = nullptr;
    QString portName;

signals:
    void constructor(Port* this_ptr);
public slots:
    void disconnectPort();
};

#endif // PORT_H
