#ifndef EFFECT_H
#define EFFECT_H

#include <QObject>
#include <QWidget>

#include "Params/parameter.h"
#include "Params/sliderparam.h"

// DEFINE EFFECTS HERE
//TODO find out how to join everything up under struct/typedef
//so it's all defined in one place.

// Link to classes in creation switch-statement.
enum EffectTypes
{
    echo1 = 0,
    fuzz,
    pan,
    end
};

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
    void addParameter(Parameter* param, QString name); //TODO Add "connect" portion (lambda expr?)
    SliderParam* addSliderParameter(QString name, int min, int max, int val);

};

#endif // EFFECT_H
