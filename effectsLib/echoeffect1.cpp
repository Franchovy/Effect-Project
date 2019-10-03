#include "echoeffect1.h"

#include <QtDebug>

EchoEffect1::EchoEffect1(QObject *parent) : Effect(parent)
{
    effectName = "Echo Effect 1";
    len = 1000;
    lenParam = new SliderParam(1000, 100000, len);
    QObject::connect(lenParam, &SliderParam::valueChanged, this, &EchoEffect1::changeLen);
    parameters.append(lenParam);

    effectBuffer = QByteArray(len, 0);
    effectBufferpt = 1;

    resizeBuffer(len);
}

void EchoEffect1::applyEffect(char *in, char *out, int readLength){
    for (int i = 0; i < readLength; i += 1){
        effectBuffer[effectBufferpt] = (effectBuffer[effectBufferpt] >> 1) + in[i];
        out[i] = effectBuffer[effectBufferpt];

        effectBufferpt += 1;
        if (effectBufferpt > len) {
            effectBufferpt = 1;
        }
    }
}

void EchoEffect1::resizeBuffer(int newSize){
    len = newSize;
    //qDebug() << "Resizing buffer";
    effectBuffer.resize(newSize);
    effectBuffer.fill(0);
}

void EchoEffect1::changeLen(int value)
{
    resizeBuffer(value);
}
