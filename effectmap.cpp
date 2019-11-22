#include "effectmap.h"

#include <QHash>
#include "effect.h"
#include "ports/port.h"

EffectMap::EffectMap(QObject *parent) : QObject(parent),
    m_effectMap(new QHash<Effect*, QList<Port*>>()),
    m_connectionsMap(new QHash<Port*, Port*>())
{

}

void EffectMap::addEffect(Effect *e)
{
    m_effectMap->insert(e, QList<Port*>());
}

void EffectMap::addPort(Effect *e, Port *p)
{
    QList<Port*> list = m_effectMap->take(e);
    list.append(p);
    m_effectMap->insert(e,list);
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
        //Remove inverse copy
        m_connectionsMap->remove(p2_unused);
    }

    //Add two connections
    m_connectionsMap->insert(p1, p2);
    m_connectionsMap->insert(p2, p1);

    //(Maybe to remove and reimplement differently)
    //Connect effects
    p1->setConnectedPort(p2);


}

QList<Port *> EffectMap::getPorts(Effect *e)
{
    return m_effectMap->value(e);
}

QList<Port *> EffectMap::getFreePorts()
{
    QList<Port*> portlist = QList<Port*>();
    {
        QList<Port*> i;
        foreach(i, m_effectMap->values()){
            portlist.append(i);
        }
    }
    {
        QList<Port*>::iterator i = QList<Port*>::iterator();
                for (i = portlist.begin(); i != portlist.end(); i++){
                    if (m_connectionsMap->contains(i.i->t())){
                        portlist.erase(i);
            }
        }
    }
    return portlist;
}
