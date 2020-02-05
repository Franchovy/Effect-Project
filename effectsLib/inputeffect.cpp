#include "inputeffect.h"

#include "ports/outport.h"
#include <QDebug>

InputEffect::InputEffect(Audio* parent) : Effect(parent)
{
    effectName = "Input Device Effect";
    type = 0;
    inputDevicePort = new OutPort("Input Device port", this);
    addPort(inputDevicePort);
    //dataLength = 200000;
    dataLength = 0;
    data = new char[dataLength];

}

void InputEffect::giveData(char *indata, int readLength)
{
    data = indata;
}

char* InputEffect::giveData(float *indata, int readLength)
{
    data = reinterpret_cast<char*>(indata);
    return data;
    /*
    if (dataLength < readLength){
        qDebug() << "Extending size";
        data = new char[readLength];
        dataLength = readLength;
    }
    qDebug() << "give data";

    memcpy(&data, indata, readLength);
*/
    /*
    for (int i = 0; i < readLength; i += 4){
        memcpy(&data[i],&indata[i],4);
    }*/
}

void InputEffect::applyEffect(char *in, char *out, int readLength)
{
    freqLimit++;
    //unused square wave
    for (int i = 0; i < readLength; i++){
        freq--;
        if (freq >= freqLimit/2){
            float x = 1;
            memcpy(&out[i],&x,4);
            //out[i] = 1;
        } else if (freq <= 400){
            float x = 0;
            memcpy(&out[i],&x,4);
            //out[i] = 0;
            if (freq == 0){
                freq = freqLimit;
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
