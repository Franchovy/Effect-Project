#include "echoeffect1.h"

#include <QtDebug>

EchoEffect1::EchoEffect1(QObject *parent) : Effect(parent)
{
    effectName = "Echo Effect 1";
    //TODO compartmentalise these
    len = 8000;
    lenParam = new SliderParam(800, 80000, len, "Buffer Length");
    QObject::connect(lenParam, &SliderParam::valueChanged, this, &EchoEffect1::changeLen);
    parameters.append(lenParam);

    delayVal = 10;
    delayParam = new SliderParam(1, 100, delayVal, "Delay speed");
    QObject::connect(delayParam, &SliderParam::valueChanged, [this](int newVal){delayVal = newVal;});
    parameters.append(delayParam);

    effectBuffer = QByteArray(len, 0);
    effectBufferpt = 1;

    resizeBuffer(len);
}

void EchoEffect1::applyEffect(char *in, char *out, int readLength){
    for (int i = 0; i < readLength; i += 1){
        effectBuffer[effectBufferpt] = in[i] + (effectBuffer[effectBufferpt] / delayVal);
        out[i] = effectBuffer[effectBufferpt];

        effectBufferpt += 1;
        if (effectBufferpt > len) {
            effectBufferpt = 1;
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

void EchoEffect1::changeLen(int value)
{
    resizeBuffer(value);
}
