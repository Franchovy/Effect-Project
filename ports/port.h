#ifndef PORT_H
#define PORT_H

#include <QObject>

QT_BEGIN_NAMESPACE
class Effect;
class QComboBox;
QT_END_NAMESPACE

class Port : public QObject
{
    Q_OBJECT
public:
    explicit Port(QString name, Effect *parent = nullptr);

    bool isConnectedPortSet();
    Port* getConnectedPort();
    void setConnectedPort(Port* port);

    QComboBox* getConnectionSelect(){return connectionSelect;}
    virtual char* getData();

    QString getName(){return portName;}
protected:
    Port* connectedPort = nullptr;
    QString portName;

    //temporary UI
    QComboBox* connectionSelect;

};

#endif // PORT_H
