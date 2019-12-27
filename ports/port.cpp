#include "port.h"


#include "GUI/gui_port.h"
#include "effect.h"
#include "effectmap.h"
#include <QComboBox>
#include <QDebug>
#include <QPushButton>

Port::Port(QString name, Effect *parent) : QObject(parent)
{
    constructor(this);
    portName = name;
    portType = -1;
}




