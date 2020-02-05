#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAbstractItemView>
#include <QAudioBuffer>
#include <QAudioProbe>
#include <QAudioRecorder>
#include <QComboBox>
#include <QFrame>
#include <QGroupBox>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QScrollBar>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QAudioRecorder>
#include <QInputDialog>

#include "effectsLib/echoeffect1.h"
#include "effectsLib/multiplyeffect.h"
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
    , m_audio(new Audio(this))
    , m_settingsDialog(new SettingsDialog(
                           m_audio,
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
    m_graphicsView->setTransformationAnchor(QGraphicsView::NoAnchor);
    m_graphicsView->scale(qreal(0.8), qreal(0.8));
    m_graphicsView->setMinimumSize(400, 400);
    m_graphicsView->setWindowTitle(tr("Effect Workspace"));
    m_graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    m_graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

    m_effectsUI->installEventFilter(this);

    m_effectsUI->setView(m_graphicsView);
    m_graphicsView->show();

    //ui->effectsSelect->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //ui->effectsSelect->setMaxVisibleItems(10); //CHANGEME doesn't work with style or smth

    // Connect Audio and UI

    runAudioUIConnections();

    // Load base effects

    ui->effectsSelect->installEventFilter(this);

    ui->effectsSelect->addItem("Input Effect");
    ui->effectsSelect->addItem("Output Effect");
    ui->effectsSelect->addItem("Echo Effect 1");
    ui->effectsSelect->addItem("Pan Effect");
    ui->effectsSelect->addItem("Wave Effect");
    ui->effectsSelect->addItem("Joiner Effect");
    ui->effectsSelect->addItem("Splitter Effect");
    ui->effectsSelect->addItem("Delay Effect");

    // Load effect files (to be updated.. to "effect container" files

    applicationPath = getApplicationPath();
    loadEffectFiles();

    ui->effectGrid->addLayout(m_effectsUI->mainLayout,0,0);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadEffectFiles()
{
    QString dirPath = getEffectFolderPath();
    QDir folder(dirPath);
    if (!folder.exists()){
        QDir().mkdir(dirPath);
    }

    resetLoadedEffects();

    //assume the directory exists and contains some files and you want all jpg and JPG files
    QStringList fileList = folder.entryList(QStringList(), QDir::Files);
    for(QString filename : fileList) {
        qDebug() << filename;
        if(readEffectFile(filename)){        
            ui->effectsSelect->addItem(filename);
        }
    }
}

void MainWindow::resetLoadedEffects()
{
    // Clears non base effects
    while (ui->effectsSelect->count() > m_audio->baseEffectsCount()){
        ui->effectsSelect->removeItem(m_audio->baseEffectsCount());
    }
    loadedEffects.clear();
}

bool MainWindow::readEffectFile(QString filename)
{
    QFile* file = new QFile(getEffectFolderPath() + "/" + filename);

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(file);

    QString line = in.readLine();

    QList<QPair<int, int>> effects;
    QList<QPair<QPair<int, int>,QPair<int,int>>> connections;

    if(line.indexOf("EFFECTS:") == 0){
        line = in.readLine().trimmed();
        do {
            //Effect to add
            if (int index = line.indexOf(":")){
                QPair<int,int> pair;
                QString i = line.left(index);
                QString j = line.right(line.length()-index-2);
                if (i != "" && j != ""){
                    pair.first = i.toInt();
                    pair.second = j.toInt();

                    if (pair.first > m_audio->baseEffectsCount()
                            || pair.second > m_audio->baseEffectsCount()){
                        //simple check
                        return false;
                    }
                    effects.append(pair);
                }
            } else return false;
            line = in.readLine().trimmed();
        } while (line != "CONNECTIONS:");
        line = in.readLine();
        while (line != ""){
            if (int index = line.indexOf(":")){
                QString left = line.left(index);
                QString right = line.mid(index+2);
                // Connections to add
                QPair<int,int> connection1;
                QPair<int,int> connection2;
                connection1.first = left.left(line.indexOf(" ")).toInt();
                connection1.second = left.mid(line.indexOf(" ")+1).toInt();

                connection2.first = right.left(line.indexOf(" ")).toInt();
                connection2.second = right.mid(line.indexOf(" ")+1).toInt();
                // basic check
                if (connection1.first > effects.length())
                    return false;
                if (connection2.first > effects.length())
                    return false;
                connections.append(QPair<QPair<int,int>,QPair<int,int>>(connection1,connection2));
                line = in.readLine();
            }
        }
    }

    loadedEffects.append(QPair<QList<QPair<int,int>>,QList<QPair<QPair<int,int>,QPair<int,int>>>>(effects, connections));
    return true;
}

void MainWindow::setEffectFolderPath(QString path)
{
    effectsFolderPath = path;
    qDebug() << "Setting path to: " << effectsFolderPath;
    loadEffectFiles();
}

QString MainWindow::getEffectFolderPath()
{
    if (effectsFolderPath == ""){
        effectsFolderPath = getApplicationPath() + "/" + effectsFolderName;
    }
    return effectsFolderPath;
}

QString MainWindow::getApplicationPath()
{
    QString path = QDir::currentPath();
    return path;
}

void MainWindow::runAudioUIConnections()
{
    // Connect new effect button to
    connect(ui->newEffectButton, &QPushButton::clicked, [=](){
        if (ui->effectsSelect->currentIndex() <  m_audio->baseEffectsCount()) {
            // Signal base effect
            qDebug() << "Base effect: " << ui->effectsSelect->currentIndex();
            m_effectsUI->newEffectSignal(ui->effectsSelect->currentIndex());
        } else {
            // Create effect from loaded effects
            qDebug() << "Loaded effect: " << ui->effectsSelect->currentIndex();
            m_effectsUI->loadEffect(loadedEffects.at(ui->effectsSelect->currentIndex() - m_audio->baseEffectsCount()));
        }
    });
    // Set update for when effects menu is clicked
    //connect(ui->effectsSelect, &QComboBox)

    // Connect all Audio slots to EffectsScene signals
    connect(m_effectsUI, &EffectsScene::newEffectSignal, m_audio, &Audio::createEffect);
    connect(m_effectsUI, &EffectsScene::deleteEffectSignal, m_audio, &Audio::deleteEffect);
    connect(m_effectsUI, &EffectsScene::connectPortsSignal, m_audio, &Audio::connectPorts);
    connect(m_effectsUI, &EffectsScene::disconnectPortsSignal, m_audio, &Audio::disconnectPorts);

    connect(m_audio, &Audio::newEffectSignal, m_effectsUI, &EffectsScene::addEffect);

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
            if (!m_audio->isAudioSystemSet()){
                m_audio->setAudioSystem(defaultAudioSystem);
            }
            ui->runButton->setText("Run Audio");
            m_audio->runAudioSignal();
        }
    });
    connect(ui->settings, &QPushButton::clicked, this, &MainWindow::showSettingsDialog);

    // Connect save effect button to EffectsScene compiler mechanism
    connect(ui->saveEffectButton, &QPushButton::clicked, this, &MainWindow::printSaveEffect);

}

void MainWindow::showSettingsDialog()
{
    m_settingsDialog->exec();
    if (m_settingsDialog->result() == QDialog::Accepted) {
        effectsFolderPath = m_settingsDialog->effectsDirectory();
        //m_audio->setInputDevice(m_settingsDialog->inputDevice());
        //m_audio->setOutputDevice(m_settingsDialog->outputDevice());
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress){
        if(obj == ui->effectsSelect){
                // Reset and refill the ComboBox
                loadEffectFiles();
            }
            return false;
        }

    if (obj == m_effectsUI && event->type() == QEvent::GraphicsSceneMouseMove)
    {
        if (m_effectsUI->getDragState() == 3) // if view drag state
        {
            QGraphicsSceneMouseEvent *m = static_cast<QGraphicsSceneMouseEvent*>(event);
            QPointF d = m->lastScreenPos() - m->screenPos();
            int newX = m_graphicsView->horizontalScrollBar()->value() + d.x();
            int newY = m_graphicsView->verticalScrollBar()->value() + d.y();
            m_graphicsView->horizontalScrollBar()->setValue(newX);
            m_graphicsView->verticalScrollBar()->setValue(newY);
            return true;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}


void MainWindow::on_toggleRecordButton_clicked()
{
    m_audio->record();
}

void MainWindow::printSaveEffect()
{
    QString folder = getEffectFolderPath();
    // QDialog for name
    bool ok;
    QString name = QInputDialog::getText(this, tr("Effect Name Dialog"),
                                             tr("Enter Effect Name:"), QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
    name.append(".effect");
    if (ok && !name.isEmpty()){
        QString outputString = m_effectsUI->compileSaveEffect();

        // Create effect file
        QFile* file = new QFile(folder + "/" + name);
        if (!file->exists()){
            if (file->open(QIODevice::ReadWrite) )
            {
                QTextStream stream(file);
                stream << outputString;
                stream.flush();
                file->close();
                qDebug() << "Saving effect: " << endl << outputString;
            }
        } else qDebug() << "Effect already exists.";
    } else {
        qDebug() << "Effect unsaved.";
    }
}
