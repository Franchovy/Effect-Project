#ifndef SPLITTEREFFECT_H
#define SPLITTEREFFECT_H

#include <QObject>
#include "effect.h"

class SplitterEffect : public Effect
{
    Q_OBJECT
public:
    SplitterEffect(Audio* parent = nullptr);

private:
    InPort* in;
    OutPort* out1;
    OutPort* out2;

    // Effect interface
public:
    void applyEffect(char *in, char *out, int readLength);
    char *getData(OutPort *, int readLength);
};

#endif // SPLITTEREFFECT_H
