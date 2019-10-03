#ifndef PARAMETER_H
#define PARAMETER_H

#include <QWidget>

class Parameter : public QObject
{
    Q_OBJECT
public:
    explicit Parameter(QObject* parent = nullptr);

    virtual QWidget* getWidget() {return widget;}

private:
    QWidget* widget;
};

#endif // PARAMETER_H
