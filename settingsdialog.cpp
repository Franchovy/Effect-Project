#include "settingsdialog.h"

#include <QAbstractItemView>
#include <QDebug>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include "audio.h"


SettingsDialog::SettingsDialog(
        Audio &audio,
        QWidget *parent) : QDialog(parent)
      , m_audioSelect(new QComboBox(this))
      , m_chooseEffectsFolderButton(new QPushButton("Select Directory", this))
      , m_inputDeviceComboBox(new QComboBox(this))
      , m_outputDeviceComboBox(new QComboBox(this))
{
    this->audio = &audio;
    const QList<QAudioDeviceInfo> &availableInputDevices = audio.availableAudioInputDevices();
    const QList<QAudioDeviceInfo> &availableOutputDevices = audio.availableAudioOutputDevices();

    QVBoxLayout *dialogLayout = new QVBoxLayout(this);
    dialogLayout->setSpacing(2);

    //Populate combo boxes

    m_audioSelect->addItem("Qt",static_cast<AudioSystem>(0));
    m_audioSelect->addItem("JACK",static_cast<AudioSystem>(1));

    QAudioDeviceInfo device;
    for (QAudioDeviceInfo device : availableInputDevices){
        m_inputDeviceComboBox->addItem(device.deviceName(), QVariant::fromValue(device));
    }
    m_inputDeviceComboBox->setMaximumWidth(150);
    m_inputDeviceComboBox->view()->setMinimumWidth(m_inputDeviceComboBox->fontMetrics().lineWidth());
    for (QAudioDeviceInfo device : availableOutputDevices){
        m_outputDeviceComboBox->addItem(device.deviceName(), QVariant::fromValue(device));
    }
    m_outputDeviceComboBox->setMaximumWidth(150);
    m_outputDeviceComboBox->view()->setMinimumWidth(m_outputDeviceComboBox->fontMetrics().lineWidth());

    //Initialize default devices
    if (!availableInputDevices.empty())
        m_inputDevice = availableInputDevices.front();
    if (!availableOutputDevices.empty())
        m_outputDevice = availableOutputDevices.front();

    //Add widgets to layout
    QHBoxLayout* audioSelectLayout(new QHBoxLayout);
    QLabel* audioSelectLabel = new QLabel(tr("Audio System"), this);
    audioSelectLayout->addWidget(audioSelectLabel);
    audioSelectLayout->addWidget(m_audioSelect);
    dialogLayout->addLayout(audioSelectLayout);

    QHBoxLayout* effectsFolderPathLayout(new QHBoxLayout);
    QLabel* effectsFolderPathLabel = new QLabel(tr("Effects Folder Path:"), this);
    effectsFolderPathLayout->addWidget(effectsFolderPathLabel);
    effectsFolderPathLayout->addWidget(m_chooseEffectsFolderButton);
    dialogLayout->addLayout(effectsFolderPathLayout);

    QHBoxLayout* inputDeviceLayout(new QHBoxLayout);
    QLabel *inputDeviceLabel = new QLabel(tr("Input Device"), this);
    inputDeviceLayout->addWidget(inputDeviceLabel);
    inputDeviceLayout->addWidget(m_inputDeviceComboBox);
    dialogLayout->addLayout(inputDeviceLayout);


    QHBoxLayout* outputDeviceLayout(new QHBoxLayout);
    QLabel *outputDeviceLabel = new QLabel(tr("Output Device"), this);
    outputDeviceLayout->addWidget(outputDeviceLabel);
    outputDeviceLayout->addWidget(m_outputDeviceComboBox);
    dialogLayout->addLayout(outputDeviceLayout);


    // Connect
    connect(m_inputDeviceComboBox, QOverload<int>::of(&QComboBox::activated),
           this, &SettingsDialog::inputDeviceChanged);
    connect(m_outputDeviceComboBox, QOverload<int>::of(&QComboBox::activated),
            this, &SettingsDialog::outputDeviceChanged);
    connect(m_chooseEffectsFolderButton, &QPushButton::pressed, [=](){
        QString Directory = QFileDialog::getExistingDirectory(this,
                                tr("Choose Or Create Directory"),
                                "/home");
    });

    // Add standard buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    dialogLayout->addWidget(buttonBox);

    // Connect standard buttons
    connect(buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked,
            this, &SettingsDialog::accept);
    connect(buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked,
            this, &SettingsDialog::reject);

    setLayout(dialogLayout);

}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::audioSystemChanged(int index)
{
    AudioSystem audioSystem = static_cast<AudioSystem>(index);
    audio->setAudioSystem(audioSystem);

}

void SettingsDialog::inputDeviceChanged(int index)
{
    m_inputDevice = m_inputDeviceComboBox->itemData(index).value<QAudioDeviceInfo>();
    audio->setInputDevice(m_inputDevice);
}

void SettingsDialog::outputDeviceChanged(int index)
{
    m_outputDevice = m_outputDeviceComboBox->itemData(index).value<QAudioDeviceInfo>();
    audio->setOutputDevice(m_outputDevice);
}
