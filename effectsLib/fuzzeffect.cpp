#include "fuzzeffect.h"

FuzzEffect::FuzzEffect(Audio* parent) : Effect(parent)
{
    effectName = "Fuzz Effect";
    fuzzVal = 10;
}

void FuzzEffect::applyEffect(char *in, char *out, int readLength)
{
    for (int i = 0; i < readLength; i++){
        out[i] = in[i] * fuzzVal;
    }
}
