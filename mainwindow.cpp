#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAudioBuffer>
#include <QAudioProbe>
#include <QAudioRecorder>
#include <QComboBox>
#include <QFrame>
#include <QGroupBox>
#include <QGraphicsView>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QAudioRecorder>

#include <experimental/filesystem>

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
    // Set up Audio

    //default in/out effects
    //Effect* e_in = m_audio->createEffect(0);
    //Effect* e_out = m_audio->createEffect(1);

    //m_effectsUI->connectPortsSignal(QPair<Effect*,int>(e_in,0), QPair<Effect*,int>(e_out,0));

    // Set up UI
    ui->setupUi(this);

    QLayout* masterLayout = ui->effectGrid->layout();
    ui->effectGrid->deleteLater();

    masterLayout->addWidget(m_graphicsView);

    m_graphicsView->setScene(m_effectsUI);
    m_graphicsView->setAttribute(Qt::WA_Hover);

    // Totally my own settings

    m_graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    m_graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_graphicsView->setRenderHint(QPainter::Antialiasing);
    m_graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_graphicsView->scale(qreal(0.8), qreal(0.8));
    m_graphicsView->setMinimumSize(400, 400);
    m_graphicsView->setWindowTitle(tr("Effect Workspace"));

    m_effectsUI->setView(m_graphicsView);
    m_graphicsView->show();

    // Load effect files (to be updated.. to "effect container" files

    applicationPath = getApplicationPath();
    //readEffectFiles();

    // Load base effects

    ui->effectsSelect->addItem("Input Effect");
    ui->effectsSelect->addItem("Output Effect");
    ui->effectsSelect->addItem("Echo Effect 1");
    ui->effectsSelect->addItem("Pan Effect");
    ui->effectsSelect->addItem("Wave Effect");
    ui->effectsSelect->addItem("Joiner Effect");

    ui->effectGrid->addLayout(m_effectsUI->mainLayout,0,0);

    // Connect Audio and UI

    runAudioUIConnections();

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readEffectFiles()
{
    QString dirPath = QString(applicationPath + "/" + effectsFolderName);
    QDir folder(dirPath);
    if (!folder.exists()){
        QDir().mkdir(dirPath);
    }

    //assume the directory exists and contains some files and you want all jpg and JPG files
    QStringList images = folder.entryList(QStringList(), QDir::Files);
    for(QString filename : images) {
        qDebug() << filename;
    }
}


QString MainWindow::getApplicationPath()
{
    QString path = QString::fromStdString(std::experimental::filesystem::current_path());
    return path;
}

void MainWindow::runAudioUIConnections()
{
    // Connect new effect button to
    connect(ui->newEffectButton, &QPushButton::clicked, [=](){
        m_effectsUI->newEffectSignal(ui->effectsSelect->currentIndex());
    });

    // Connect all Audio slots to EffectsScene signals
    connect(m_effectsUI, &EffectsScene::newEffectSignal, m_audio, &Audio::createEffect);
    connect(m_effectsUI, &EffectsScene::deleteEffectSignal, m_audio, &Audio::deleteEffect);
    connect(m_effectsUI, &EffectsScene::connectPortsSignal, m_audio, &Audio::connectPorts);
    connect(m_effectsUI, &EffectsScene::disconnectPortsSignal, m_audio, &Audio::disconnectPorts);

    connect(m_audio, &Audio::newEffectSignal, m_effectsUI, &EffectsScene::addEffect);

    connect(ui->runButton, &QPushButton::pressed, [=](){
        if (m_audio->isRunning()){
            m_audio->runAudio();
        } else {
            m_audio->stopAudio();
        }
    });
    connect(m_audio, &Audio::runAudioSignal, m_effectsUI, &EffectsScene::runAudioSignal);
    connect(m_effectsUI, &EffectsScene::runAudioSignal, m_audio, &Audio::runAudio);
    connect(m_audio, &Audio::runAudioSignal, [=](){
        ui->runButton->setText("Stop Audio");
    });
    connect(m_audio, &Audio::stopAudioSignal, m_effectsUI, &EffectsScene::stopAudioSignal);
    connect(m_effectsUI, &EffectsScene::stopAudioSignal, m_audio, &Audio::stopAudio);
    connect(m_audio, &Audio::stopAudioSignal, [=](){
        ui->runButton->setText("Run Audio");
    });

    connect(ui->runButton, &QPushButton::pressed, [=](){
        if (m_audio->isRunning()){
            ui->runButton->setText("Stop");
            m_audio->stopAudioSignal();
        } else {
            ui->runButton->setText("Run Audio");
            m_audio->runAudioSignal();
        }
    });
    connect(ui->settings, &QPushButton::clicked, this, &MainWindow::showSettingsDialog);

}

void MainWindow::showSettingsDialog()
{
    m_settingsDialog->exec();
    if (m_settingsDialog->result() == QDialog::Accepted) {
        m_audio->setInputDevice(m_settingsDialog->inputDevice());
        m_audio->setOutputDevice(m_settingsDialog->outputDevice());
    }
}




void MainWindow::on_toggleRecordButton_clicked()
{
    m_audio->record();
}
