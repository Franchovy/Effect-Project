#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QComboBox>
#include <QFrame>
#include <QGroupBox>
#include <QGraphicsView>
#include <QDebug>

#include "effectsLib/echoeffect1.h"
#include "effectsLib/fuzzeffect.h"
#include "effectsLib/paneffect.h"

#include "effect.h"
#include "effectmap.h"
#include "audio.h"
#include "ports/port.h"
#include "settingsdialog.h"
#include "effectsscene.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_audio(new Audio())
    , m_settingsDialog(new SettingsDialog(
                           *m_audio,
                           this))
    , m_effectsUI(new EffectsScene(this))
    , m_graphicsView(new QGraphicsView(this))
{
    m_audio->setUI(m_effectsUI);

    // UI SETUP
    ui->setupUi(this);

    QLayout* masterLayout = ui->effectGrid->layout();
    ui->effectGrid->deleteLater();

    masterLayout->addWidget(m_graphicsView);

    m_graphicsView->setScene(m_effectsUI);
    m_graphicsView->setAttribute(Qt::WA_Hover);
    //Copied settings

    m_graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    m_graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_graphicsView->setRenderHint(QPainter::Antialiasing);
    m_graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_graphicsView->scale(qreal(0.8), qreal(0.8));
    m_graphicsView->setMinimumSize(400, 400);
    m_graphicsView->setWindowTitle(tr("Effect Workspace"));

    m_effectsUI->setView(m_graphicsView);
    m_graphicsView->show();



    m_effectsUI->setupEffectsSelect(ui->effectsSelect);
    ui->effectGrid->addLayout(m_effectsUI->mainLayout,0,0);

    //Connect Audio and UI
    connect(m_audio->getEffectMap(), &EffectMap::constructor, m_effectsUI, &EffectsScene::effect_constructor);
    connect(m_effectsUI,&EffectsScene::connectPortsSignal, m_audio->getEffectMap(), &EffectMap::connectPorts);

    connect(ui->newEffectButton, &QPushButton::pressed, [=](){
        int effectType = m_effectsUI->getNewEffectType();

        Effect* e = m_audio->createEffect(effectType);

        switch(effectType){
        case 0: //Input effect
            break;
        case 1: //Output effect
            break;
        default:
            m_effectsUI->addEffect(e);
            m_audio->addEffect(e);
        }

        //connect to delete operation
        connect(e,&Effect::destroyed,[=](){
            //Delete effect audio
            m_audio->removeEffect(e);
            //Delete effect UI
            m_effectsUI->deleteEffect(e);
        });
    });
    connect(ui->runButton, &QPushButton::pressed, m_audio, &Audio::runAudio);
    connect(ui->settings, &QPushButton::clicked, this, &MainWindow::showSettingsDialog);

    //Create default in and out effects
    Effect* e_in = m_audio->createEffect(0);
    Effect* e_out = m_audio->createEffect(1);

    m_effectsUI->connectPortsSignal(e_in->getPorts().first(), e_out->getPorts().first());
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showSettingsDialog()
{
    m_settingsDialog->exec();
    if (m_settingsDialog->result() == QDialog::Accepted) {
        m_audio->setInputDevice(m_settingsDialog->inputDevice());
        m_audio->setOutputDevice(m_settingsDialog->outputDevice());
    }
}
