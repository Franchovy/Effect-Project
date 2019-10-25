#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QComboBox>
#include <QFrame>
#include <QGroupBox>
#include "effectsLib/fuzzeffect.h"
#include "effectsLib/paneffect.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    // UI SETUP
    ui->setupUi(this);

    populateSelectEffects();
    ui->effectsSelect->setEditText("Select Effect");
    connect(ui->effectsSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::effectsSelectIndexChanged);

    audio = new Audio(); //init

    reloadEffectChainUI();

    connect(ui->newEffectButton, &QPushButton::pressed, this, &MainWindow::createEffect);

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
    audio->getEffectBuffer()->addEffect(e);
    reloadEffectChainUI();
}
