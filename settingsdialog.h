#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QAudioDeviceInfo>

QT_BEGIN_NAMESPACE
class Audio;
class MainWindow;
class QComboBox;
class QDialogButtonBox;
class QPushButton;
class QVBoxLayout;
QT_END_NAMESPACE

class SettingsDialog : public QDialog
{
public:
    SettingsDialog(Audio *audio,
                   QWidget *parent = nullptr);
    ~SettingsDialog();

    const QString effectsDirectory() const {return m_effectsDirectory; }
    const QAudioDeviceInfo &inputDevice() const { return m_inputDevice; }
    const QAudioDeviceInfo &outputDevice() const { return m_outputDevice; }


private:
    Audio *audio;
    QComboBox *m_audioSelect;

    QPushButton *m_chooseEffectsFolderButton;
    QString m_effectsDirectory;

    QAudioDeviceInfo m_inputDevice;
    QAudioDeviceInfo m_outputDevice;

    QComboBox *m_inputDeviceComboBox;
    QComboBox *m_outputDeviceComboBox;


private Q_SLOTS:
    void audioSystemChanged(int index);
    void inputDeviceChanged(int index);
    void outputDeviceChanged(int index);

};

#endif // SETTINGSDIALOG_H
