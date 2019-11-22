#include "inport.h"

#include "outport.h"
#include <QComboBox>
#include <QDebug>

QList<InPort*>* InPort::inportList = new QList<InPort*>();

InPort::InPort(QString name, Effect* parent) : Port(name, parent)
{
    inportList->append(this);
    connect(connectionSelect,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int i){
        if (i != -1)
            connectedPort = OutPort::getOutPortList()->at(i);
    });

    //TODO find a way to compartmentalise this in port class. Passing OutPort as a Port ...
    QList<OutPort*>* outportList = OutPort::getOutPortList();
    /*
    connectionSelect->clear();
    for (int i = 0; i < outportList->length(); i++){
        connectionSelect->addItem(outportList->at(i)->getName());
    }
    */
}

InPort::~InPort()
{
    inportList->removeOne(this);
}

char *InPort::getData()
{
    if (connectedPort != nullptr){
        return connectedPort->getData();
    } else return nullptr;
}


void InPort::updateConnectionSelect()
{
    QList<OutPort*>* outportList = OutPort::getOutPortList();
    connectionSelect->clear();
    for (int i = 0; i < outportList->length(); i++){
        connectionSelect->addItem(outportList->at(i)->getName());
    }
}
