#include "sliderparam.h"

#include <QToolTip>
#include <QSlider>
#include <effect.h>
#include <QGroupBox>
#include <QDebug>

SliderParam::SliderParam(int min, int max, int val, Effect *parent) : Parameter(parent)
{   
    this->min = min;
    this->max = max;
    this->val = val;

    sliderWidget = new QSlider();
    sliderWidget->setMinimum(min);
    sliderWidget->setMaximum(max);
    sliderWidget->setValue(val);
    sliderWidget->setToolTip(this->objectName());
    widget = sliderWidget;

    QObject::connect(sliderWidget, &QSlider::sliderMoved, [this](int val){
        sliderWidget->setToolTip(getName().append(" = ").append(QString::number(val)));
        QToolTip::showText(sliderWidget->mapToGlobal( QPoint( 0, 0 ) ),  sliderWidget->toolTip());
    });

    QObject::connect(sliderWidget, &QSlider::sliderReleased, [this](){
        sliderWidget->setToolTip(getName());
        emit this->valueChanged(sliderWidget->value());
    });
}

