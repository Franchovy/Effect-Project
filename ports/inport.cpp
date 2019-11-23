#include "inport.h"

#include "effectmap.h"
#include "effect.h"
#include "outport.h"
#include <QComboBox>
#include <QDebug>


InPort::InPort(QString name, Effect* parent) : Port(name, parent)
{
    const int portType = 1;

    Port::setupConnectionSelect(parent->getEffectMap()->getFreePortsOfType(portType));
}

char *InPort::getData()
{
    if (connectedPort != nullptr){
        return connectedPort->getData();
    } else return nullptr;
}
