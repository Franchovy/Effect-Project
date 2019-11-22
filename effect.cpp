#include "effect.h"

#include <QDebug>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include "ports/inport.h"
#include "ports/outport.h"


Effect::Effect(QObject* parent) : QObject(parent)
{

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

void Effect::setConnectedPort(Port *port1, Port *port2)
{
    // Sets connection both ways.
    port1->setConnectedPort(port2);
}

QGroupBox* Effect::generateUI()
{
    qDebug() << "Generating UI.";
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

    InPort *inP;
    foreach (inP, inPortList) {
        portSelectLayout->addWidget(new QLabel(inP->getName()));
        portSelectLayout->addWidget(inP->getConnectionSelect());
    }

    portSelectLayout->addWidget(new QLabel("OutPort"));
    OutPort *outP;
    foreach (outP, outPortList) {
        portSelectLayout->addWidget(new QLabel(outP->getName()));
        portSelectLayout->addWidget(outP->getConnectionSelect());
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
    parameters.append(param);
}

//Meant to be the same as "addParameter" but got stuck trying to use child class parameters.
//TODO try again and make sure it wasn't just a #include problem.
SliderParam* Effect::addSliderParameter(QString name, int min, int max, int val){
    SliderParam *param = new SliderParam(min, max, val, this);
    param->setObjectName(name);
    parameters.append(param);

    return param;
}

char *Effect::getData()
{
    return nullptr;
}


void Effect::updatePortConnectionSelects()
{
    InPort *inPort;
    foreach(inPort, inPortList){
        inPort->updateConnectionSelect();
    }
    OutPort *outPort;
    foreach(outPort, outPortList){
        outPort->updateConnectionSelect();
    }
}
