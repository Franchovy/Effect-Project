#include "outport.h"

#include "effect.h"
#include "inport.h"
#include <QComboBox>
#include <QDebug>

QList<OutPort*>* OutPort::outportList = new QList<OutPort*>();

OutPort::OutPort(QString name, Effect* parent) : Port(name, parent)
{
    this->parent = parent;

    outportList->append(this);
    connect(connectionSelect,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int i){
        if (i != -1)
            connectedPort = InPort::getInPortList()->at(i);
    });
    //TODO find a way to compartmentalise this in port class. Passing InPort as a Port ...
    /*
    QList<InPort*>* inportList = InPort::getInPortList();
    for (int i = 0; i < inportList->length(); i++){
        connectionSelect->addItem(inportList->at(i)->getName());
    }
    */

}

OutPort::~OutPort()
{
    outportList->removeOne(this);
}

char *OutPort::getData()
{
    return parent->getData();
}

void OutPort::updateConnectionSelect()
{
    QList<InPort*>* inportList = InPort::getInPortList();
    connectionSelect->clear();
    for (int i = 0; i < inportList->length(); i++){
        connectionSelect->addItem(inportList->at(i)->getName());
    }
}
