#include "inputeffect.h"

#include "ports/outport.h"
#include <QDebug>

InputEffect::InputEffect(EffectMap* parent) : Effect(parent)
{
    effectName = "Input Device Effect";
    inputDevicePort = new OutPort("Input Device port", this);
    m_outPortList.append(inputDevicePort);
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
