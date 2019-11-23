#include "port.h"

#include "effect.h"
#include <QComboBox>

Port::Port(QString name, Effect *parent) : QObject(parent),
    portType(0)
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

void Port::setupConnectionSelect(QList<Port *> selectList)
{
    //Connect the connectionSelect
    connect(connectionSelect,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int i){
        if (i != -1)
            connectedPort = selectList.at(i);
    });

    //Populate connectionSelect
    //TODO to compartmentalise - and call from multiple sources.
    for (int i = 0; i < selectList.length(); i++){
        connectionSelect->addItem(selectList.at(i)->getName());
    }
}

char *Port::getData()
{
    return nullptr;
}



