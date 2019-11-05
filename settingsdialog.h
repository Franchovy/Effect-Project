#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QAudioDeviceInfo>

QT_BEGIN_NAMESPACE
class QComboBox;
class QDialogButtonBox;
class QPushButton;
class QVBoxLayout;
QT_END_NAMESPACE

class SettingsDialog : public QDialog
{
public:
    SettingsDialog(const QList<QAudioDeviceInfo> &availableInputDevices,
                   const QList<QAudioDeviceInfo> &availableOutputDevices,
                   QWidget *parent = nullptr);
    ~SettingsDialog();

    const QAudioDeviceInfo &inputDevice() const { return m_inputDevice; }
    const QAudioDeviceInfo &outputDevice() const { return m_outputDevice; }

private slots:
    void inputDeviceChanged(int index);
    void outputDeviceChanged(int index);

private:
    QAudioDeviceInfo m_inputDevice;
    QAudioDeviceInfo m_outputDevice;

    QComboBox *m_inputDeviceComboBox;
    QComboBox *m_outputDeviceComboBox;
};

#endif // SETTINGSDIALOG_H
