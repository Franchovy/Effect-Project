#include "echoeffect1.h"

#include <QtDebug>
#include "ports/inport.h"
#include "ports/outport.h"
#include "Params/sliderparam.h"


EchoEffect1::EchoEffect1(Audio* parent) : Effect(parent)
{
    effectName = "Echo Effect 1";

    type = 2;

    InPort* inPort = new InPort("Echo In", this);
    OutPort* outPort = new OutPort("Echo Out", this);

    addPort(inPort);
    addPort(outPort);

    /*
    lenParam = addSliderParameter("Buffer Length", 800, 80000, len = 8000);
    QObject::connect(lenParam, &SliderParam::valueChanged, [this](int value){
        resizeBuffer(value);
    });

    delayParam = addSliderParameter("Delay speed", 1, 100, delayVal = 10);
    QObject::connect(delayParam, &SliderParam::valueChanged, [this](int newVal){
        delayVal = newVal;
    });
    */

    effectBufferpt = 0;
    /*effectBuffer = QByteArray(len, 0);
    effectBuffer.fill(0);*/

    delayOpVal = 1.5;
    resizeBuffer(5000);
}
//In and Out data buffers
void EchoEffect1::applyEffect(char *in, char *out, int readLength){
    for (int i = 0; i < readLength; i += step){
        if (format.sampleType() == QAudioFormat::SignedInt){
            int16_t x;
            memcpy(&x, &in[i], step);
            int16_t b;
            memcpy(&b, &effectBuffer[effectBufferpt], step);
            x += b;
            x = static_cast<int16_t>(x / delayOpVal);
            memcpy(&out[i], &x, step);
            memcpy(&effectBuffer[effectBufferpt], &x, step);
        } else if (format.sampleType() == QAudioFormat::Float){
            float x;
            memcpy(&x, &in[i], step);
            float b;
            memcpy(&b, &effectBuffer[effectBufferpt], step);
            x += b;
            x = static_cast<float>(x / delayOpVal);
            memcpy(&out[i], &x, step);
            memcpy(&effectBuffer[effectBufferpt], &x, step);
        }
        effectBufferpt += step;
        if (effectBufferpt > len) {
            effectBufferpt = 0;
        }
    }
}

char *EchoEffect1::getData(OutPort*, int readLength)
{
    // Only one outport
    char* outData = getOutPortData(outPorts.first(), readLength);
    //TODO resize buffer here based on readLength;

    EchoEffect1::applyEffect(inPorts.first()->getData(), outData, readLength); //TODO check if ok
    return outData;
}

void EchoEffect1::resizeBuffer(int newSize){
    newSize *= 8;
    len = newSize;

    //qDebug() << "Resizing buffer";
    effectBuffer = new char[len];
    //effectBuffer.resize(newSize);
    //effectBuffer.fill(0);
}

