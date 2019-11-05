#include "effect.h"

#include <QDebug>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>

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

SliderParam* Effect::addSliderParameter(QString name, int min, int max, int val){
    SliderParam *param = new SliderParam(min, max, val, this);
    param->setObjectName(name);
    parameters.append(param);

    return param;
}
