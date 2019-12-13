#include "outport.h"

#include "effectmap.h"
#include "effect.h"
#include "port.h"
#include "inport.h"
#include <QComboBox>
#include <QDebug>


OutPort::OutPort(QString name, Effect* parent) : Port(name, parent)
{
    parentEffect = parent;
    setupConnectionSelect();
}

char *OutPort::getData()
{
    return parentEffect->getData();
}
