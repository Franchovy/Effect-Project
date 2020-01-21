#ifndef SLIDERPARAM_H
#define SLIDERPARAM_H

#include "parameter.h"

QT_BEGIN_NAMESPACE
class QSlider;
class Effect;
class QGroupBox;
QT_END_NAMESPACE

class SliderParam : public Parameter
{
    Q_OBJECT
public:
    explicit SliderParam(int min, int max, int val, Effect* parent);

private:
    int min, max, val;
    QSlider* sliderWidget;

Q_SIGNALS:
    void valueChanged(int newValue);

};

#endif // SLIDERPARAM_H
