#include "parameter.h"

Parameter::Parameter(QString name, QObject *parent) : QObject(parent)
{
    this->name = name;
}

QWidget *Parameter::getWidget() {
    return widget;
}
