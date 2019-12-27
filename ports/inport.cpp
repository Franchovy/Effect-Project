#include "inport.h"

#include "effectmap.h"
#include "effect.h"
#include "outport.h"
#include <QComboBox>
#include <QDebug>


InPort::InPort(QString name, Effect* parent) : Port(name, parent)
{
    portType = 0;
    parentEffect = parent;
}

char *InPort::getData()
{
    parentEffect->effectMap->getData(parentEffect);
}
