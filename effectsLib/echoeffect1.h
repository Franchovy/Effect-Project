#ifndef ECHOEFFECT1_H
#define ECHOEFFECT1_H

#include <effect.h>

#include "Params/sliderparam.h"

class EchoEffect1 : public Effect
{
    Q_OBJECT
public:
    explicit EchoEffect1(QObject *parent = nullptr);

    void applyEffect(char *in, char *out, int readLength);

private:
    SliderParam* lenParam = nullptr;
    int len;

    SliderParam* delayParam = nullptr;
    int delayVal;
    double delayOpVal;

    QByteArray effectBuffer;
    int effectBufferpt;

    void resizeBuffer(int newSize);
};

#endif // ECHOEFFECT1_H
