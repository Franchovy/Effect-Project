#ifndef FLANGEREFFECT_H
#define FLANGEREFFECT_H

#include "effect.h"

#include <Params/sliderparam.h>

class FlangerEffect : public Effect
{
    Q_OBJECT
public:
    explicit FlangerEffect(Audio* parent = nullptr);

    virtual void applyEffect(char* in, char* out, int readLength);

private:
    int strength;
    int velocity;
    int length;

    SliderParam* strengthParam = nullptr;
    SliderParam* velocityParam = nullptr;
    SliderParam* lengthParam = nullptr;

    QByteArray flangerBuffer;
    int inBufferPt;
    int outBufferPt;
    bool fwd;

    void resizeBuffer(int newSize);
};

#endif // FLANGEREFFECT_H
