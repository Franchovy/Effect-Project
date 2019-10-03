#include "sliderparam.h"

SliderParam::SliderParam(int min, int max, int val, QObject *parent) : Parameter(parent)
{
    this->min = min;
    this->max = max;
    this->val = val;

    widget = new QSlider();
    widget->setMinimum(min);
    widget->setMaximum(max);
    widget->setValue(val);

    QObject::connect(widget, &QSlider::sliderReleased, [this](){
        emit this->valueChanged(widget->value());
    });
}

QSlider* SliderParam::getWidget()
{
    return widget;
}

