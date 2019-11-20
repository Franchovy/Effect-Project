#include "inputeffect.h"

#include "ports/outport.h"
#include <QDebug>

InputEffect::InputEffect(QObject *parent) : Effect(parent)
{
    inputDevicePort = new OutPort("Input Device port", this);
}

void InputEffect::giveData(char *data)
{
    this->data = data;
    hasData = true;
}

void InputEffect::applyEffect(char *in, char *out, int readLength)
{
    out = in;
}

char *InputEffect::getData()
{
    if (hasData){
        hasData = false;
        return data;
    } else {
        qWarning() << "Error, has no data!";
        return nullptr;
    }
}
