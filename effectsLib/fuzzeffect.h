#ifndef FUZZEFFECT_H
#define FUZZEFFECT_H

#include "effect.h"

#include <Params/sliderparam.h>

class FuzzEffect : public Effect
{
    Q_OBJECT
public:
    explicit FuzzEffect(EffectMap* parent = nullptr);

public:
    void applyEffect(char *in, char *out, int readLength);

private:
    int fuzzVal;
    SliderParam* fuzzParam;
};

#endif // FUZZEFFECT_H
