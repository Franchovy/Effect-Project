#include "echoeffect1.h"

#include <QtDebug>

EchoEffect1::EchoEffect1()
{
    len = 50000;
    effectBuffer = QByteArray(len, 0);

    effectBufferpt = 1;

    if (effectBuffer.length() < len) {
        qDebug() << "Resizing buffer";
        effectBuffer.resize(len);
        effectBuffer.fill(0);
    }
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
