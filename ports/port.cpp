#include "port.h"

#include "effect.h"
#include "effectmap.h"
#include <QComboBox>
#include <QPushButton>

Port::Port(QString name, Effect *parent) : QObject(parent)
{
    this->parentEffect = parent;

    connectionSelect = new QComboBox();
    disconnectButton = new QPushButton();

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

void Port::setupConnectionSelect()
{
    connect(this, &Port::sendConnectionSelect, parentEffect->getEffectMap(), &EffectMap::updatePortConnectionSelect);
    connect(disconnectButton, &QPushButton::clicked, this, &Port::disconnectPort);
}

char *Port::getData()
{
    return nullptr;
}

void Port::disconnectPort()
{
    connectedPort = nullptr;
}




