#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFrame>
#include <QDebug>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    audio = new Audio(); //init

    reloadEffectChainUI();

    connect(ui->newEffectButton, &QPushButton::pressed, [this](){
        Effect* e = new Effect();
        audio->getEffectBuffer()->addEffect(e);
        reloadEffectChainUI();
                });
    connect(ui->runButton, &QPushButton::pressed, audio, &Audio::runAudio);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::reloadEffectChainUI()
{
    QList<Effect*>* effectsChain = audio->getEffectChain();

    QGroupBox* frame;
    for (int i = 0; i < effectsChain->length(); i++){

        frame = new QGroupBox();

        frame->setGeometry(QRect(15+(i%8)*120, 15+(i/8)*120, 100, 100)); //doesn't work, rip

        ui->effectGrid->addWidget(frame, i/8, i%8);
    }
}

