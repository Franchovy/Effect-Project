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

}

void EffectMap::addEffect(Effect *e)
{
    m_effectMap->insert(e,e->getPorts());
}

void EffectMap::deleteEffect(Effect *e)
{
    // Remove Connections
    for (Port* p : e->getPorts()){
        if (m_connectionsMap->contains(p)){
            p = m_connectionsMap->take(p);
            m_connectionsMap->take(p);
        }
    }
    // Remove Effect
    m_effectMap->remove(e);
}

void EffectMap::connectPorts(Port *p1, Port *p2)
{
    if (m_connectionsMap->contains(p1)){
        p1 = m_connectionsMap->take(p1);
        p1 = m_connectionsMap->take(p1);
    }
    if (m_connectionsMap->contains(p2)){
        p2 = m_connectionsMap->take(p2);
        p2 = m_connectionsMap->take(p2);
    }
    m_connectionsMap->insert(p1, p2);
    m_connectionsMap->insert(p2, p1);
}

void EffectMap::disconnectPorts(Port *p1, Port *p2)
{
    if (m_connectionsMap->contains(p1) && m_connectionsMap->contains(p2)){
        if (
                (m_connectionsMap->find(p1).value() == p2) &&
                (m_connectionsMap->find(p2).value() == p1)){
            m_connectionsMap->remove(p1);
            m_connectionsMap->remove(p2);
        }
    }
}


