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
    /*
    if (dataLength < readLength){
        //TODO something wrong here
        this->data = new char[readLength];
        dataLength = readLength;
        qDebug() << "New char";
    }
    for (int i = 0; i < readLength; i++){
        this->data[i] = data[i];
    }
    //memcpy(this->data, data, readLength);
    hasData = true;
    */
    this->data = data;

}

void InputEffect::giveData(float *dataFloat, int readLength)
{
    if (sizeof(this->data) < readLength){
        this->data = new char[readLength];
    }
    memcpy(data, dataFloat, readLength);

    //qDebug() << "Data: " << static_cast<char>(data[0]);
    //qDebug() << "FloatData: " << static_cast<char>(dataFloat[0]);

    /*
    for (int i = 0; i < readLength; i++){
        this->data[i] = dataFloat[i];

        qDebug() << "Data size: " << sizeof(dataFloat[i]);
        qDebug() << "Float:";
        qDebug() << hex << dataFloat[i];

    }*/
    hasData = true;
}

void InputEffect::applyEffect(char *in, char *out, int readLength)
{
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
