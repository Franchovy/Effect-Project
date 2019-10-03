#ifndef SLIDERPARAM_H
#define SLIDERPARAM_H

#include "parameter.h"

#include <QSlider>

class SliderParam : public Parameter
{
    Q_OBJECT
public:
    explicit SliderParam(int min, int max, int val, QObject* parent = nullptr);

    QSlider* getWidget();

private:
    int min, max, val;
    QSlider* widget;

signals:
    void valueChanged(int newValue);

};

#endif // SLIDERPARAM_H
