#ifndef PARAMETER_H
#define PARAMETER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class Effect;
QT_END_NAMESPACE

class Parameter : public QObject
{
    Q_OBJECT
public:
    explicit Parameter(Effect* parent);

    QWidget* getWidget();

    QString getName(){return name;}
protected:
    QString name;
    QWidget* widget;
};

#endif // PARAMETER_H
