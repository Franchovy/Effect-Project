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


Effect::Effect(EffectMap* parent) : QObject(parent)
  , m_inPortList(QList<InPort*>())
  , m_outPortList(QList<OutPort*>())
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
    QList<Port *> list = QList<Port *>();
    Port* p;
    foreach(p, m_inPortList){
        list.append(p);
    }
    foreach(p, m_outPortList){
        list.append(p);
    }
    return list;
}


QGroupBox* Effect::generateUI()
{
    frame = new QGroupBox();
    frame->setTitle(effectName);

    QHBoxLayout* layout = new QHBoxLayout(frame);
    frame->setLayout(layout);

    for (Parameter* p : *(getParamList())){
        layout->addWidget(p->getWidget());
    }

    // delete button
    deleteButton = new QPushButton(frame);
    layout->addWidget(deleteButton);
    connect(deleteButton, &QPushButton::pressed, [=](){
        deleteLater();
    });


    // Input/output port comboBoxes
    QVBoxLayout* portSelectLayout = new QVBoxLayout();
    layout->addLayout(portSelectLayout);

    portSelectLayout->addWidget(new QLabel("InPort"));
    for(InPort* inP : m_inPortList){
        portSelectLayout->addWidget(new QLabel(inP->getName()));
        portSelectLayout->addWidget(inP->getConnectionSelect());
        portSelectLayout->addWidget(inP->getDisconnectButton());
    }

    portSelectLayout->addWidget(new QLabel("OutPort"));
    for(OutPort* outP : m_outPortList){
        portSelectLayout->addWidget(new QLabel(outP->getName()));
        portSelectLayout->addWidget(outP->getConnectionSelect());
        portSelectLayout->addWidget(outP->getDisconnectButton());
    }

    bUIGenerated = true;
    return frame;
}

bool Effect::isUIGenerated()
{
    return bUIGenerated;
}

/**
 * @brief Effect::getUI
 * @return nullptr if UI is not yet generated.
 */
QGroupBox* Effect::getUI()
{
    return frame;
}

void Effect::addParameter(Parameter *param, QString name)
{
    param = new Parameter(this);

    param->setObjectName("name");
    m_parameters.append(param);
}

//Meant to be the same as "addParameter" but got stuck trying to use child class parameters.
//TODO try again and make sure it wasn't just a #include problem.
SliderParam* Effect::addSliderParameter(QString name, int min, int max, int val){
    SliderParam *param = new SliderParam(min, max, val, this);
    param->setObjectName(name);
    m_parameters.append(param);

    return param;
}

char *Effect::getData()
{
    return nullptr;
}

