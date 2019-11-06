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

    Port* getConnectedPort() {return connectedPort;}

    QString getName(){return portName;}
protected:
    Port* connectedPort = nullptr;
    virtual char* getData();
    QString portName;


    //temporary UI
    QComboBox* connectionSelect;
};

#endif // PORT_H
