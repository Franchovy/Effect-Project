#include "port.h"

#include "effect.h"
#include <QComboBox>

Port::Port(QString name, Effect *parent) : QObject(parent)
{
    connectionSelect = new QComboBox();
    portName = name;
}

char *Port::getData()
{
    return nullptr;
}



