#ifndef PARAMETER_H
#define PARAMETER_H

#include <QWidget>

class Parameter : public QObject
{
    Q_OBJECT
public:
    explicit Parameter(QString name, QObject* parent = nullptr);

    virtual QWidget* getWidget();

    QString getName(){return name;}
private:
    QString name;
    QWidget* widget;
};

#endif // PARAMETER_H
