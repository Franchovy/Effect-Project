#include "inputeffect.h"

#include "ports/outport.h"
#include <QDebug>

InputEffect::InputEffect(Audio* parent) : Effect(parent)
{
    effectName = "Input Device Effect";
    type = 0;
    inputDevicePort = new OutPort("Input Device port", this);
    addPort(inputDevicePort);
    data = {};
}

void InputEffect::giveData(char *data, int readLength)
{
    this->data = data;
}


void InputEffect::applyEffect(char *in, char *out, int readLength)
{
    //unused square wave
    for (int i = 0; i < readLength; i++){
        freq--;
        if (freq >= 400){
            out[i] = 1;
        } else if (freq <= 400){
            out[i] = 0;
            if (freq == 0){
                freq = 800;
            }
        }
    }
}

char *InputEffect::getData(OutPort*, int readLength)
{
    //char* outData = getOutPortData(outPorts.first(), readLength);
    //applyEffect(nullptr, outData, readLength);
    return data;
}
