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

private:
    static QList<OutPort*>* outportList;
    Effect* parent;

//public slots:
    void updateConnectionSelect();
};


#endif // OUTPORT_H
