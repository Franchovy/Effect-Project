#ifndef SPLITTER_H
#define SPLITTER_H

#include <QObject>
#include "effect.h"

class WaveEffect : public Effect
{
    Q_OBJECT
public:
    explicit WaveEffect(Audio* parent = nullptr);

private:
    InPort* inPort;
    OutPort* outPort;

    double vol;
    double dvol = 0.05;
    int dir = 1;

    // Effect interface
public:
    void applyEffect(char *in, char *out, int readLength);
    char *getData(OutPort *, int);
};

#endif // SPLITTER_H
