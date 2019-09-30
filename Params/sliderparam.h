#ifndef SLIDERPARAM_H
#define SLIDERPARAM_H

#include "parameter.h"

#include <QSlider>

class SliderParam : public Parameter
{
public:
    SliderParam(int min, int max, int val);

    QSlider* getWidget();

private:
    int min, max, val;
    QSlider* widget;

};

#endif // SLIDERPARAM_H
