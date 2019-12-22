#include "inputeffect.h"

#include "ports/outport.h"
#include <QDebug>

InputEffect::InputEffect(Audio* parent) : Effect(parent)
{
    effectName = "Input Device Effect";
    inputDevicePort = new OutPort("Input Device port", this);
    ports.append(inputDevicePort);
}

void InputEffect::giveData(char *data)
{

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
