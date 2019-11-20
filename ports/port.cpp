#include "port.h"

#include "effect.h"
#include <QComboBox>

Port::Port(QString name, Effect *parent) : QObject(parent)
{
    connectionSelect = new QComboBox();
    portName = name;
}

bool Port::isConnectedPortSet()
{
    if (connectedPort != nullptr){
        return true;
    } else {
        return false;
    }
}

void Port::setConnectedPort(Port *port)
{
    if (!isConnectedPortSet()){
        connectedPort = port;
        port->setConnectedPort(this);
    }
}

char *Port::getData()
{
    return nullptr;
}



