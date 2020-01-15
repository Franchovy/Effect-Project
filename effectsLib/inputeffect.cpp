#include "inputeffect.h"

#include "ports/outport.h"
#include <QDebug>

InputEffect::InputEffect(Audio* parent) : Effect(parent)
{
    effectName = "Input Device Effect";
    type = 0;
    inputDevicePort = new OutPort("Input Device port", this);
    addPort(inputDevicePort,QPointF(50,100));
    data = {};
}

void InputEffect::giveData(char *data, int readLength)
{
    if (sizeof(this->data) < readLength){
        this->data = new char[readLength];
    }
    for (int i = 0; i < readLength; i++){
        this->data[i] = data[i];
    }
    //memcpy(this->data, data, readLength);
    hasData = true;
}

void InputEffect::giveData(float *dataFloat, int readLength)
{
    /* Figure out length to assert
    if (sizeof(this->data) < readLength){
        this->data = new char[readLength];
    }*/
    for (int i = 0; i < readLength; i++){
        this->data[i] = dataFloat[i]; //TODO
    }
    hasData = true;
}

void InputEffect::applyEffect(char *in, char *out, int readLength)
{
    out = in;
}

char *InputEffect::getData(OutPort*, int readLength)
{
    if (hasData){
        hasData = false;
        return data;
    } else {
        qWarning() << "Error, has no data!";
        return nullptr;
    }
}
