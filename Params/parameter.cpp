#include "parameter.h"

#include <effect.h>

Parameter::Parameter(Effect *parent) : QObject(parent)
{
    //WARNING: QObject should always be of type Effect. How to enforce this?
}

QWidget *Parameter::getWidget() {
    return widget;
}
