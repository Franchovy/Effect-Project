#include "effectmap.h"

#include <QComboBox>
#include <QHash>
#include <QDebug>

#include "audio.h"
#include "effect.h"
#include "effectbuffer.h"
#include "effectsLib/inputeffect.h"
#include "effectsLib/outputeffect.h"
#include "ports/port.h"
#include "ports/outport.h"
#include "ports/inport.h"

#include <effectsLib/echoeffect1.h>
#include <effectsLib/fuzzeffect.h>
#include <effectsLib/paneffect.h>

EffectMap::EffectMap(Audio *parent) : QObject(parent),
    m_effectMap(new QHash<Effect*, QList<Port*>>()),
    m_connectionsMap(new QHash<Port*, Port*>())
{
    parentAudio = parent;
    //createDefaultInputOutputEffects(inputEffect, outputEffect);
}

void EffectMap::addEffect(Effect *e)
{    
    qDebug() << "Add Effect [EffectMap]";
    QList<Port*> list = QList<Port*>();
    for (Port* p: e->getPorts()){
        list.append(p);
    }
    m_effectMap->insert(e, list);

    for (Port* p: e->getPorts()){
        p->sendConnectionSelect(p->getConnectPortType());
    }
}

Effect* EffectMap::createEffect(int effectType)
{
    qDebug() << "Create Effect [EffectMap]";
    Effect* e;
    switch(effectType){
    case 0:
        e = new InputEffect(this);
        break;
    case 1:
        e = new OutputEffect(this);
        break;
    case 2:
        e = new EchoEffect1(this);
        break;
    case 3:
        e = new PanEffect(this);
        break;
    case 4:
        e = new FuzzEffect(this);
        break;
    default:
        e = new Effect(); // useless default effect
    }

    constructor(e);
    return e;
}


void EffectMap::connectPorts(Port *p1, Port *p2)
{
    Port* p1_unused = m_connectionsMap->value(p1);
    Port* p2_unused = m_connectionsMap->value(p2);

    if (p1_unused != nullptr){ //WARNING check if this works
        //Remove if connected
        m_connectionsMap->take(p1);
        //Remove inverse copy
        m_connectionsMap->remove(p1_unused);
    }
    if (p2_unused != nullptr){ //WARNING check if this works
        //Remove if connected
        m_connectionsMap->take(p2);
        //Remove inverse copy    portType(0)

        m_connectionsMap->remove(p2_unused);
    }

    //Add two connections
    m_connectionsMap->insert(p1, p2);
    m_connectionsMap->insert(p2, p1);

    //Connect effects
    p1->setConnectedPort(p2);
}

QList<Port *> EffectMap::getPorts(Effect *e)
{
    return m_effectMap->value(e);
}

QList<Port *> EffectMap::getPorts()
{
    QList<Port*> portlist = QList<Port*>();
    QList<QList<Port*>> list = m_effectMap->values();

    for(QList<Port*> i : list){
        portlist.append(i);
    }
    return portlist;
}

QList<Port *> EffectMap::getPortsOfType(int type)
{
    QList<Port *> list = QList<Port*>();
    QList<Port *> portlist = getPorts();
    for (Port* p : portlist){
        if (p->getPortType() == type){
            list.append(p);
        }
    }
    return list;
}

QList<Port *> EffectMap::getFreePorts()
{
    QList<Port*> list = QList<Port*>();
    for (Port* p : getPorts()){
        if (!m_connectionsMap->contains(p)){
            list.append(p);
        }
    }
    return list;
}

QList<Port *> EffectMap::getFreePortsOfType(int type)
{
    QList<Port *> list = QList<Port*>();
    for(Port* p : getFreePorts()){
        if (p->getPortType() == type){
            list.append(p);
        }
    }
    return list;
}
/*
void EffectMap::createDefaultInputOutputEffects(InputEffect *inputEffect, OutputEffect *outputEffect)
{


    //Create and add input/output device
    inputEffect = new InputEffect(this);
    outputEffect = new OutputEffect(this);

    addEffect(inputEffect);
    addEffect(outputEffect);
    //connectPorts(inputEffect->inputDevicePort, outputEffect->outputDevicePort);

    parentAudio->getEffectBuffer()->addInputEffect(inputEffect);
    parentAudio->getEffectBuffer()->addOutputEffect(outputEffect);
}
*/

/**
 * @brief EffectMap::updatePortConnectionSelect Updates connectionSelect lists of ports of given type.
 * @param type Port type to be updated - fetches "connectPortType" of type to populate list.
 */
void EffectMap::updatePortConnectionSelect(int type)
{
    QList<Port*> portList = getPortsOfType(type);
    QList<Port*> selectList;
    if (portList.length() > 0){
        selectList = getFreePortsOfType(portList.first()->getConnectPortType());
    }

    for (Port* port : portList){
        //TODO separate this out to avoid redefining at every update
        //Connect the connectionSelect
        connect(port->getConnectionSelect(),
                QOverload<int>::of(&QComboBox::currentIndexChanged),
                [=](int i){
            if (i != -1)
                port->setConnectedPort(selectList.at(i));
        });
        port->getConnectionSelect()->clear();

        for (Port* port2 : selectList){
            port->getConnectionSelect()->addItem(port2->getName());
        }
    }
}
