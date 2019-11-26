#ifndef OUTPORT_H
#define OUTPORT_H

#include <QObject>
#include "port.h"

QT_BEGIN_NAMESPACE
class InPort;
class Effect;
class QComboBox;
QT_END_NAMESPACE

class OutPort : public Port
{
    Q_OBJECT
public:
    OutPort(QString name, Effect* parent);

    char* getData() override;
    int getPortType() override {return 2;}
    int getConnectPortType() override {return 1;}

private:
    static QList<OutPort*>* outportList;
    Effect* parentEffect;

//public slots:
    void updateConnectionSelect();
};


#endif // OUTPORT_H
