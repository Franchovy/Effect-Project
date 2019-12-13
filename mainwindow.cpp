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
#include "audio.h"
#include "settingsdialog.h"
#include "effectsscene.h"


#define GRAPHICS_UI

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
    // UI SETUP
    ui->setupUi(this);

#ifdef GRAPHICS_UI
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

    m_graphicsView->show();

    m_effectsUI->setupEffectsSelect(ui->effectsSelect);
    ui->effectGrid->addLayout(m_effectsUI->mainLayout,0,0);

#else
    m_effectsUI->setupEffectsSelect(ui->effectsSelect);
    ui->effectGrid->addLayout(m_effectsUI->mainLayout,0,0);
#endif

    connect(ui->newEffectButton, &QPushButton::pressed, [=](){
        int effectType = m_effectsUI->getNewEffectType();
        Effect* e = m_audio->createEffect(effectType);
        m_effectsUI->createEffectUI(e);
        m_audio->addEffect(e);

        //connect to delete operation
        connect(e,&Effect::destroyed,[=](){
            //Delete effect audio
            m_audio->removeEffect(e);
            //Delete effect UI
            m_effectsUI->deleteEffectUI(e);
        });
    });
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
