#include "sliderparam.h"

#include <QToolTip>
#include <effect.h>

SliderParam::SliderParam(int min, int max, int val, QObject *parent) : Parameter(parent)
{   
    this->min = min;
    this->max = max;
    this->val = val;

    widget = new QSlider();
    widget->setMinimum(min);
    widget->setMaximum(max);
    widget->setValue(val);
    widget->setToolTip(this->objectName());

    QObject::connect(widget, &QSlider::sliderMoved, [this](int val){
        widget->setToolTip(getName().append(" = ").append(QString::number(val)));
        QToolTip::showText( widget->mapToGlobal( QPoint( 0, 0 ) ),  widget->toolTip());
    });

    QObject::connect(widget, &QSlider::sliderReleased, [this](){
        widget->setToolTip(getName());
        emit this->valueChanged(widget->value());
    });
}

QSlider* SliderParam::getWidget()
{
    return widget;
}

