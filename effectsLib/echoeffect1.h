#ifndef ECHOEFFECT1_H
#define ECHOEFFECT1_H

#include <effect.h>

#include "Params/sliderparam.h"

class EchoEffect1 : public Effect
{
public:
    EchoEffect1();

    void applyEffect(char *in, char *out, int readLength);

private:
    SliderParam lenParam;

    QByteArray effectBuffer;
    int effectBufferpt;
    int len;
};

#endif // ECHOEFFECT1_H
