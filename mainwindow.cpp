#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFrame>
#include <QGroupBox>
#include "effectsLib/fuzzeffect.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    audio = new Audio(); //init

    reloadEffectChainUI();

    connect(ui->newEffectButton, &QPushButton::pressed, [this](){ //Wow! Check out this totally PRO lambda-expression!
        Effect* e = new EchoEffect1();
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
    //for (int i = 0; i < effectsChain->length(); i++){
    int i = 0;
    for (Effect* e : *effectsChain){
        frame = new QGroupBox();
        frame->setTitle(effectsChain->at(i)->effectName);

        QVBoxLayout* layout = new QVBoxLayout();
        layout->setGeometry(QRect(15+(i%8)*120, 15+(i/8)*120, 100, 100));

        frame->setLayout(layout);

        ui->effectGrid->addWidget(frame, i/8, i%8);

        for (Parameter* p : *(e->getParamList())){
            QWidget* w = p->getWidget();

            layout->addWidget(w);
        }
        i++;
    }
}

