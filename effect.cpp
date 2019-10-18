#include "effect.h"

#include <QDebug>

Effect::Effect(QObject* parent) : QObject(parent)
{

}

void Effect::applyEffect(char *in, char *out, int readLength)
{
    //default stream. Like no effect
    for (int i = 0; i < readLength; i++){
        out[i] = in[i];
    }
}

void Effect::addParameter(Parameter *param, QString name)
{
    param = new Parameter(this);

    param->setObjectName("name");
    parameters.append(param);
}

SliderParam* Effect::addSliderParameter(QString name, int min, int max, int val){
    SliderParam *param = new SliderParam(min, max, val, this);
    param->setObjectName(name);
    parameters.append(param);

    return param;
}
