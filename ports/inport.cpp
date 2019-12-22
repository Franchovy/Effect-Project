#include "inport.h"

#include "effectmap.h"
#include "effect.h"
#include "outport.h"
#include <QComboBox>
#include <QDebug>


InPort::InPort(QString name, Effect* parent) : Port(name, parent)
{

}

char *InPort::getData()
{
    if (connectedPort != nullptr){
        return connectedPort->getData();
    } else return nullptr;
}
