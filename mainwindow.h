#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "audio.h"

QT_BEGIN_NAMESPACE
class Effect;
class EffectsScene;
class Audio;
class SettingsDialog;
class QGraphicsView;
QT_END_NAMESPACE


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setEffectFolderPath(QString path);

private:
    AudioSystem defaultAudioSystem = QT;
    AudioSystem audioSystem;

    Ui::MainWindow *ui;
    Audio* m_audio;
    SettingsDialog *m_settingsDialog;
    EffectsScene* m_effectsUI;

    // lol, need to learn how to use a struct
    QList<QPair<QList<QPair<int,int>>,QList<QPair<QPair<int,int>,QPair<int,int>>>>> loadedEffects;

    void loadEffectFiles();
    void resetLoadedEffects();
    bool readEffectFile(QString filename);
    QString effectsFolderPath = "";
    QString effectsFolderName = "Effects";
    QString getEffectFolderPath();
    QString getApplicationPath();
    QString applicationPath;

    QGraphicsView* m_graphicsView;
    bool eventFilter(QObject *obj, QEvent *event);

    void runAudioUIConnections();

private Q_SLOTS:
    void showSettingsDialog();
    void on_toggleRecordButton_clicked();
    void printSaveEffect();
};
#endif // MAINWINDOW_H
