#include "effectsui.h"

#include <QDebug>
#include <QGridLayout>
#include <QComboBox>
#include <QGroupBox>

#include "effect.h"
#include "effectmap.h"

#include "effectsLib/echoeffect1.h"
#include "effectsLib/fuzzeffect.h"
#include "effectsLib/paneffect.h"

EffectsUI::EffectsUI(QWidget *parent) : QWidget(parent)
    , mainLayout(new QGridLayout)
{

}

void EffectsUI::setupEffectsSelect(QComboBox* effectsSelect)
{
    effectTypeList.append("Echo");
    effectTypeList.append("Pan");
    effectTypeList.append("Fuzz");
    effectsSelect->insertItems(0, effectTypeList);

    effectsSelect->setEditText("Select Effect");

    connect(effectsSelect,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int i){
        newEffectType = i;
    });
}

void EffectsUI::createEffectUI(Effect* e)
{
    if (!e->isUIGenerated()){
        QGroupBox* w = e->generateUI();
        mainLayout->addWidget(w);
    }
}

void EffectsUI::deleteEffectUI(Effect *e)
{
    mainLayout->removeWidget(e->getUI());
    e->getUI()->deleteLater(); //Should not have to do this?
    mainLayout->update();
}


int EffectsUI::getNewEffectType() const
{
    return newEffectType;
}


