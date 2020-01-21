#ifndef JOINEREFFECT_H
#define JOINEREFFECT_H

#include <QObject>
#include "effect.h"

class JoinerEffect : public Effect
{
    Q_OBJECT
public:
    JoinerEffect(Audio *parent = nullptr);

private:
    InPort* in1;
    InPort* in2;
    OutPort* out;

    bool addMode = false;

    // Effect interface
public:
    void applyEffect(char *in, char *out, int readLength);
    char *getData(OutPort *, int readLength);
};

#endif // JOINEREFFECT_H
