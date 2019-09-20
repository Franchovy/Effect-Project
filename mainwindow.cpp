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

    effectChain = new QList<Effect*>();
    Effect* firstEffect = new Effect();
    effectChain->append(firstEffect);
    effectChain->append(firstEffect); //not very gud

    reloadEffectChainUI();

    audio = new Audio(); //init

    connect(ui->test, SIGNAL(pressed()), audio, SLOT(runAudio()));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::reloadEffectChainUI()
{
    QGroupBox* frame;
    for (int i = 0; i < effectChain->length(); i++){

        frame = new QGroupBox();

        frame->setGeometry(QRect(15+(i%8)*120, 15+(i/8)*120, 100, 100)); //doesn't work, rip

        ui->effectGrid->addWidget(frame, i/8, i%8);
    }
}

