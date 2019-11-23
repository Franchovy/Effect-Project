#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QComboBox>
#include <QFrame>
#include <QGroupBox>
#include <QDebug>

#include "effectsLib/echoeffect1.h"
#include "effectsLib/fuzzeffect.h"
#include "effectsLib/paneffect.h"

#include "effect.h"
#include "audio.h"
#include "settingsdialog.h"
#include "effectsui.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_audio(new Audio())
    , m_settingsDialog(new SettingsDialog(
                           *m_audio,
                           this))
    , m_effectsUI(new EffectsUI(this))
{
    // UI SETUP
    ui->setupUi(this);

    m_effectsUI->setupEffectsSelect(ui->effectsSelect);
    ui->effectGrid->addLayout(m_effectsUI->mainLayout,0,0);

    connect(ui->newEffectButton, &QPushButton::pressed, this, &MainWindow::createEffect);
    connect(ui->runButton, &QPushButton::pressed, m_audio, &Audio::runAudio);
    connect(ui->settings, &QPushButton::clicked, this, &MainWindow::showSettingsDialog);
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

void MainWindow::createEffect(){
    Effect* e;
    switch(m_effectsUI->getNewEffectType()){
    case 0:
        e = new EchoEffect1();
        break;
    case 1:
        e = new PanEffect();
        break;
    case 2:
        e = new FuzzEffect();
        break;
    default:
        e = new Effect(); // useless default effect
    }

    qDebug() << e;

    m_effectsUI->createEffectUI(e);
    m_audio->addEffect(e);

    //connect to delete operation
    connect(e,&Effect::destroyed,[=](){
        //Delete effect audio
        m_audio->removeEffect(e);
        //Delete effect UI
        m_effectsUI->deleteEffectUI(e);
    });
}
