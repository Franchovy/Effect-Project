#include "inport.h"

#include "outport.h"
#include <QComboBox>

QList<InPort*>* InPort::inportList = new QList<InPort*>();

InPort::InPort(QString name, Effect* parent) : Port(name, parent)
{
    inportList->append(this);

    //TODO find a way to compartmentalise this in port class. Passing OutPort as a Port ...
    QList<OutPort*>* outportList = OutPort::getOutPortList();
    for (int i = 0; i < outportList->length(); i++){
        connectionSelect->addItem(outportList->at(i)->getName());
    }
    connect(connectionSelect,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int i){
        connectedPort = OutPort::getOutPortList()->at(i);
    });
}

char *InPort::getData()
{
    return nullptr;
}
