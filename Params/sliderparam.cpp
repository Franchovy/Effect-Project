#include "sliderparam.h"

SliderParam::SliderParam(int min, int max, int val)
{
    this->min = min;
    this->max = max;
    this->val = val;

    widget = new QSlider();

}

QSlider* SliderParam::getWidget()
{
    return widget;
}
