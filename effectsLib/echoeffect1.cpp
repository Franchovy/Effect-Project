#include "echoeffect1.h"

#include <QtDebug>

EchoEffect1::EchoEffect1(QObject *parent) : Effect(parent)
{
    effectName = "Echo Effect 1";

    lenParam = addSliderParameter("Buffer Length", 800, 80000, len = 8000);
    QObject::connect(lenParam, &SliderParam::valueChanged, [this](int value){
        resizeBuffer(value);
    });

    delayParam = addSliderParameter("Delay speed", 1, 100, delayVal = 10);
    QObject::connect(delayParam, &SliderParam::valueChanged, [this](int newVal){
        delayVal = newVal;
    });

    effectBuffer = QByteArray(len, 0);
    effectBufferpt = 1;
    effectBuffer.fill(0);

    resizeBuffer(len);
}

void EchoEffect1::applyEffect(char *in, char *out, int readLength){
    for (int i = 0; i < readLength; i += 1){

        effectBuffer[effectBufferpt-1] = in[i] + (effectBuffer[effectBufferpt] / static_cast<float>(delayVal));// / delayVal);//* delayOpVal);
        out[i] = effectBuffer[effectBufferpt];//effectBuffer[effectBufferpt];

        effectBufferpt += 1;
        if (effectBufferpt > len) {
            effectBufferpt = 2;
        }
    }
}

void EchoEffect1::resizeBuffer(int newSize){
    newSize *= 8;
    len = newSize;

    //qDebug() << "Resizing buffer";
    effectBuffer.resize(newSize);
    effectBuffer.fill(0);
}

