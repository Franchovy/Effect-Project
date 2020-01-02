 #include "effect.h"

#include <QDebug>
#include <QList>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

#include "audio.h"
#include "effectmap.h"
#include "ports/port.h"
#include "ports/inport.h"
#include "ports/outport.h"
#include "GUI/gui_effect.h"


Effect::Effect(Audio* parent) : QObject(parent)
  , m_ports(new QMap<Port*, QPointF>())
  , m_parameters(QList<Parameter*>())
{
    effectMap = parent->getEffectMap();
    effectName = "Default Effect Name";
    type = -1;
}

void Effect::applyEffect(char *in, char *out, int readLength)
{
    //default stream/ no effect
    for (int i = 0; i < readLength; i++){
        out[i] = in[i];
    }
}

//In and Out data buffers are the same
void Effect::applyEffect(char *data, int readLength){
    applyEffect(data, data, readLength);
}

QList<QPointF> Effect::getPortLocs()
{
    return m_ports->values();
}

QList<Port *> Effect::getPorts()
{
    return m_ports->keys();
}

char *Effect::getData(OutPort*, int)
{
    return nullptr;
}

void Effect::addPort(Port *port, QPointF p)
{
    m_ports->insert(port, p);
    if (port->portType == 0){
        inPorts.append(static_cast<InPort*>(port));
    } else if (port->portType == 1){
        outPorts.append(static_cast<OutPort*>(port));
    }
}

