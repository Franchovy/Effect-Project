#ifndef EFFECT_H
#define EFFECT_H

#include <QWidget>

//UI Element of Effect. Not much to be done here, kid.
class Effect
{
public:
    Effect();

    virtual void applyEffect(char* in, char* out, int readLength){}

};

#endif // EFFECT_H
