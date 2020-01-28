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
  , m_ports(QVector<Port*>())
  , m_parameters(QList<Parameter*>())
{
    effectMap = parent->getEffectMap();
    effectName = "Default Effect Name";
    type = -1;

    format = parent->getAudioFormat();

    step = format.sampleSize()/8;
    qDebug() << "Step: " << step;
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

QList<Port *> Effect::getPorts()
{
    return m_ports.toList();
}

char *Effect::getData(OutPort*, int readLength)
{
    return nullptr;
}

void Effect::addPort(Port *port)
{
    m_ports.append(port);
    if (port->portType == 0){
        inPorts.append(static_cast<InPort*>(port));
    } else if (port->portType == 1){
        outPorts.append(static_cast<OutPort*>(port));
    }
}

char* Effect::getOutPortData(Port* port, int readLength){
    char* outData = port->data;
    if (port->dataLength < readLength){
        // Initialise data container in output port
        outData = new char[readLength];
        // Copy over any data needed
        //memcpy(outData, port->data, port->dataLength);
        // Set new Outport data
        port->data = outData;
        port->dataLength = readLength;
    }
    return outData;
}

float Effect::getFloat(char* c){
    float x = 0;
    memcpy(&x, c, 4);
    return x;
}

int16_t Effect::getInt(char* c){
    int x = 0;
    memcpy(&x, c, 2);
    return x;
}
