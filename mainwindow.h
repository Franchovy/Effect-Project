#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    Audio* m_audio;
    SettingsDialog *m_settingsDialog;
    EffectsScene* m_effectsUI;

    QGraphicsView* m_graphicsView;

private slots:
    void showSettingsDialog();
};
#endif // MAINWINDOW_H
