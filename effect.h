#ifndef EFFECT_H
#define EFFECT_H

#include <QWidget>

#include "Params/parameter.h"

class Effect
{
public:
    Effect();

    virtual void applyEffect(char* in, char* out, int readLength){}
    QString effectName = "default effect";

    QList<Parameter>* getParamList() {return &parameters;}

protected:
    QList<Parameter> parameters;
};

#endif // EFFECT_H
