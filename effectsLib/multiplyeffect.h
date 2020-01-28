#ifndef FUZZEFFECT_H
#define FUZZEFFECT_H

#include "effect.h"

#include <Params/sliderparam.h>

class MultiplyEffect : public Effect
{
    Q_OBJECT
public:
    explicit MultiplyEffect(Audio* parent = nullptr);

private:
    double multiplyVar;
    SliderParam* fuzzParam;

    // Effect interface
public:
    char *getData(OutPort *, int readLength);
    void applyEffect(char *in, char *out, int readLength);
};

#endif // FUZZEFFECT_H
