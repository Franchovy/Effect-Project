#ifndef OUTPORT_H
#define OUTPORT_H

#include <QObject>
#include "port.h"

QT_BEGIN_NAMESPACE
class InPort;
class QComboBox;
QT_END_NAMESPACE

class OutPort : public Port
{
    Q_OBJECT
public:
    OutPort(QString name, Effect* parent);
    static QList<OutPort*>* getOutPortList(){return outportList;}

private:
    static QList<OutPort*>* outportList;

    // Port interface
protected:
    char *getData() override;
};


#endif // OUTPORT_H
