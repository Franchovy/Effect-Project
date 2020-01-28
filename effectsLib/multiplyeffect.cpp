#include "multiplyeffect.h"

MultiplyEffect::MultiplyEffect(Audio* parent) : Effect(parent)
{
    effectName = "Fuzz Effect";
    multiplyVar = 0.5;
}

char *MultiplyEffect::getData(OutPort *, int readLength)
{
    char* outData = getOutPortData(outPorts.first(), readLength);
    MultiplyEffect::applyEffect(inPorts.first()->getData(), outData, readLength); //TODO check if ok
    return outData;
}

void MultiplyEffect::applyEffect(char *in, char *out, int readLength)
{
    for (int i = 0; i < readLength; i += step){
        if (format.sampleType() == QAudioFormat::SignedInt){
            int16_t x1;
            memcpy(&x1, &in[i], step);
            x1 *= multiplyVar;
            memcpy(&out[i], &x1, step);
        }
        if (format.sampleType() == QAudioFormat::Float){
            float x1;
            memcpy(&x1, &in[i], step);
            x1 *= multiplyVar;
            memcpy(&out[i], &x1, step);
        }
    }
}
