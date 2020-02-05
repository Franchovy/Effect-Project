#ifndef DELAYEFFECT_H
#define DELAYEFFECT_H

#include <QObject>
#include <effect.h>

class DelayEffect : public Effect
{
public:
    DelayEffect(Audio* parent = nullptr);

private:
    InPort* in;
    OutPort* out;
    int delayAmount = 300000;
    int delayCounter;
    bool start = true;

    char* buffer;
    int bufferLength = 0;
    int bufferpt = 0;

    // Effect interface
public:
    void applyEffect(char *in, char *out, int readLength);
    char *getData(OutPort *, int readLength);
};

#endif // DELAYEFFECT_H
