#include "port.h"

#include "effect.h"
#include "effectmap.h"
#include <QComboBox>
#include <QDebug>
#include <QPushButton>

Port::Port(QString name, Effect *parent) : QObject(parent)
{
    this->parentEffect = parent;
    constructor(this);
    portName = name;

    connectionSelect = new QComboBox();
    disconnectButton = new QPushButton();
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
    //connect(this, &Port::sendConnectionSelect, parentEffect->getEffectMap(), &EffectMap::updatePortConnectionSelect);
    //connect(disconnectButton, &QPushButton::clicked, this, &Port::disconnectPort);
}

char *Port::getData()
{
    return nullptr;
}

GUI_port *Port::getUI()
{
    return ui;
}

void Port::setUi(GUI_port *value)
{
    ui = value;
}

void Port::disconnectPort()
{
    connectedPort = nullptr;
}




