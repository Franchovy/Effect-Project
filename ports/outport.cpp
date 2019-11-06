#include "outport.h"

#include "inport.h"
#include <QComboBox>

QList<OutPort*>* OutPort::outportList = new QList<OutPort*>();

OutPort::OutPort(QString name, Effect* parent) : Port(name, parent)
{
    outportList->append(this);

    //TODO find a way to compartmentalise this in port class. Passing InPort as a Port ...
    QList<InPort*>* inportList = InPort::getInPortList();
    for (int i = 0; i < inportList->length(); i++){
        connectionSelect->addItem(inportList->at(i)->getName());
    }
    connect(connectionSelect,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int i){
        connectedPort = InPort::getInPortList()->at(i);
    });

}

char *OutPort::getData()
{
    return nullptr;
}
