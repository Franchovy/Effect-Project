#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class Effect;
class Audio;
class SettingsDialog;
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

    void reloadEffectChainUI();

private:
    Ui::MainWindow *ui;

    Audio* audio;

    SettingsDialog *m_settingsDialog;

    void populateSelectEffects();
    QList<QString> effectList;
    int newEffectType = 0;

    void createEffect();
private slots:
    void effectsSelectIndexChanged(int index);
    void showSettingsDialog();
};
#endif // MAINWINDOW_H
