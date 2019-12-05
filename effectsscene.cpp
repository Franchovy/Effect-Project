#include "effectsscene.h"

#include <QDebug>
#include <QGridLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "effect.h"
#include "effectmap.h"
#include "ports/port.h"
#include "GUI/effectgui.h"

#include "effectsLib/echoeffect1.h"
#include "effectsLib/fuzzeffect.h"
#include "effectsLib/paneffect.h"


#define GRAPHICS_UI


EffectsScene::EffectsScene(QWidget *parent) : QGraphicsScene(parent)
    , mainLayout(new QGridLayout())
    , m_effects(QList<EffectGUI*>())
{
    //Default settings or whatever
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect(-200, -200, 400, 400);
    setBackgroundBrush(QColor(255,255,255));


}

void EffectsScene::setupEffectsSelect(QComboBox* effectsSelect)
{
    effectTypeList.append("Input");
    effectTypeList.append("Output");
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

void EffectsScene::createEffectUI(Effect* e)
{
    if (!e->isUIGenerated()){
#ifdef GRAPHICS_UI
    EffectGUI* e_gui = new EffectGUI(e->effectName);
    e_gui->setPos(QPoint(0,0));
    addItem(e_gui);

#else
        QGroupBox* w = e->generateUI();
        mainLayout->addWidget(w);
#endif
    }
}

void EffectsScene::deleteEffectUI(Effect *e)
{
    mainLayout->removeWidget(e->getUI());
    e->getUI()->deleteLater(); //Should not have to do this?
    mainLayout->update();
}


int EffectsScene::getNewEffectType() const
{
    return newEffectType;
}





