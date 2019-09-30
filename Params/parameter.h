#ifndef PARAMETER_H
#define PARAMETER_H

#include <QWidget>

class Parameter
{
public:
    Parameter();

    virtual QWidget* getWidget() {return widget;}

private:
    QWidget* widget;
};

#endif // PARAMETER_H
