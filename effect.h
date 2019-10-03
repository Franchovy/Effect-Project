#ifndef EFFECT_H
#define EFFECT_H

#include <QObject>
#include <QWidget>

#include "Params/parameter.h"

class Effect : public QObject
{
    Q_OBJECT
public:
    explicit Effect(QObject *parent = nullptr);

    virtual void applyEffect(char* in, char* out, int readLength);
    QString effectName = "default effect";

    //"Non-static protected memers can not be accessed via a pointer to the base class." wot>?
    QList<Parameter*>* getParamList() {return &parameters;}

protected:
    QList<Parameter*> parameters;
};

#endif // EFFECT_H
