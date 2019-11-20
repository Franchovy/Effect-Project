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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , audio(new Audio())
    , m_settingsDialog(new SettingsDialog(
                           *audio,
                           this))
{
    // UI SETUP
    ui->setupUi(this);

    populateSelectEffects();
    ui->effectsSelect->setEditText("Select Effect");
    connect(ui->effectsSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::effectsSelectIndexChanged);
    reloadEffectChainUI();

    connect(ui->newEffectButton, &QPushButton::pressed, this, &MainWindow::createEffect);
    connect(ui->runButton, &QPushButton::pressed, audio, &Audio::runAudio);
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
        audio->setInputDevice(m_settingsDialog->inputDevice());
        audio->setOutputDevice(m_settingsDialog->outputDevice());
    }
}

void MainWindow::reloadEffectChainUI()
{
    for (Effect* e : *audio->getEffectChain()){
        if (!e->isUIGenerated()){
            ui->effectGrid->addWidget(e->generateUI());

            connect(e,&Effect::destroyed,[=](){
                //Remove remaining stuff (UI, all ptrs)
                audio->removeEffect(e);
                ui->effectGrid->removeWidget(e->getUI());
                e->getUI()->deleteLater(); //Should not have to do this?
                ui->effectGrid->update();
                reloadEffectChainUI();
            });
        }
        e->updatePortConnectionSelects();
    }
}

void MainWindow::populateSelectEffects(){
    effectList.append("Echo");
    effectList.append("Pan");
    effectList.append("Fuzz");
    ui->effectsSelect->insertItems(0, effectList);
    //ui->effectsSelect->setItemText(0, "Select Effect");
}

void MainWindow::effectsSelectIndexChanged(int index){
    newEffectType = index;
}

void MainWindow::createEffect(){
    Effect* e;
    switch(newEffectType){
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

    audio->addEffect(e);
    reloadEffectChainUI();
}
