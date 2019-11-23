#include "outport.h"

#include "effectmap.h"
#include "effect.h"
#include "port.h"
#include "inport.h"
#include <QComboBox>
#include <QDebug>



OutPort::OutPort(QString name, Effect* parent) : Port(name, parent)
{
    const int portType = 2;

    Port::setupConnectionSelect(parent->getEffectMap()->getFreePortsOfType(portType));
}

char *OutPort::getData()
{
    return parent->getData();
}
