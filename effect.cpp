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
  , m_parameters(QList<Parameter*>())
{
    effectName = "Default Effect Name";
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
    return ports;
}

char *Effect::getData()
{
    return nullptr;
}

