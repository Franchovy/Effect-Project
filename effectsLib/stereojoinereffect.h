#ifndef STEREOJOINEREFFECT_H
#define STEREOJOINEREFFECT_H

#include "effect.h"

#include <QObject>

class StereoJoinerEffect : public Effect
{
public:
    StereoJoinerEffect(Audio* parent = nullptr);

private:
    InPort* in1;
    InPort* in2;
    OutPort* out;

    int stereocount = 0;

    // Effect interface
public:
    char *getData(OutPort *, int readLength);
    void applyEffect(char *in1, char *in2, char *out, int readLength);
};

#endif // STEREOJOINEREFFECT_H
