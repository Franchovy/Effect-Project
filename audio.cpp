#include "audio.h"

#include <QDebug>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QAudio>

#include <QAudioEncoderSettings>
#include <QAudioRecorder>
#include <QAudioProbe>
#include <QAudioBuffer> //
#include <QFileDialog>

#include "effect.h"
#include "effectmap.h"
#include "effectbuffer.h"
#include "effectsLib/inputeffect.h"
#include "effectsLib/outputeffect.h"
#include "ports/inport.h"
#include "ports/outport.h"


static QVector<qreal> getBufferLevels(const QAudioBuffer &buffer);

template <class T>
static QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels);

Audio::Audio(QObject* parent) : QObject(parent)
    , m_buffer(new EffectBuffer(this))
    , m_effectMap(new EffectMap(this))
    , m_audioRecorder(new QAudioRecorder(this))
    , m_audioProbe(new QAudioProbe(this))
{

    inputDevice = new QAudioDeviceInfo(QAudioDeviceInfo::defaultInputDevice());
    outputDevice = new QAudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());

    // Set up recording

    m_audioProbe->setSource(m_audioRecorder);

    m_audioRecorder = new QAudioRecorder(this);
    m_audioProbe = new QAudioProbe(this);


    setupFormat();
}

Effect *Audio::createEffect(int effectType)
{
    if (effectType == 0){
        InputEffect* e = m_effectMap->createInputEffect();
        m_buffer->addInputEffect(e);
        return e;
    } else if (effectType == 1){
        OutputEffect* e = m_effectMap->createOutputEffect();
        m_buffer->addOutputEffect(e);
        return e;
    } else {
        Effect* e = m_effectMap->createEffect(effectType);
        return e;
    }
}

void Audio::addEffect(Effect *e)
{
    m_effectMap->addEffect(e);
}

void Audio::removeEffect(Effect* e)
{
    m_buffer->removeEffect(e);
}


QList<QAudioDeviceInfo> Audio::availableAudioInputDevices()
{
    return QAudioDeviceInfo::availableDevices(QAudio::Mode::AudioInput);
}

QList<QAudioDeviceInfo> Audio::availableAudioOutputDevices()
{
    return QAudioDeviceInfo::availableDevices(QAudio::Mode::AudioOutput);
}

void Audio::setupFormat()
{
    QAudioFormat format;
    format.setSampleRate(96000);
    format.setChannelCount(2);
    format.setSampleSize(32);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleSize(QAudioFormat::SignedInt);

    format = inputDevice->nearestFormat(format);
    format = outputDevice->nearestFormat(format);

    inputAudio = new QAudioInput(*inputDevice, format, this);
    outputAudio = new QAudioOutput(*outputDevice, format, this);
}

void Audio::setInputDevice(QAudioDeviceInfo device)
{
    inputDevice = new QAudioDeviceInfo(device);
    setupFormat();
}

void Audio::setOutputDevice(QAudioDeviceInfo device)
{
    outputDevice = new QAudioDeviceInfo(device);
    setupFormat();
}

void Audio::record(){
    if (m_audioRecorder->state() == QMediaRecorder::StoppedState) {
        m_audioRecorder->setAudioInput(inputDevice->deviceName());

        QString fileName = QFileDialog::getSaveFileName();

        m_audioRecorder->setOutputLocation(QUrl::fromLocalFile(fileName));

        QString container;

        for (auto n : m_audioRecorder->supportedContainers()){
            //qDebug() << n;
            container = n;

        }

        QAudioEncoderSettings settings;

        for (auto a : m_audioRecorder->supportedAudioCodecs()){
            //qDebug() << a;
            settings.setCodec(a);
        }
        //default settings - full settings example in audiorecorder project

        settings.setSampleRate(96000);
        //settings.setBitRate(128000);
        settings.setChannelCount(2);
        settings.setQuality(QMultimedia::VeryHighQuality);
        settings.setEncodingMode(QMultimedia::ConstantQualityEncoding);


        m_audioRecorder->setEncodingSettings(settings, QVideoEncoderSettings(), container);
        m_audioRecorder->record();
    }
}



EffectsScene *Audio::getUI()
{
    return UI;
}

void Audio::setUI(EffectsScene *ui)
{
    UI = ui;
}

bool Audio::runAudio()
{
    if (!running) {
        qDebug() << "Input Device: " << inputDevice->deviceName();
        qDebug() << "Output Device: " << outputDevice->deviceName();

        qDebug() << "Audio Running!";
        running = true;

        m_buffer->open(QIODevice::ReadWrite);

        //outputAudio->start(inputAudio->start());

        inputAudio->start(m_buffer);
        outputAudio->start(m_buffer);

        qDebug() << inputAudio->state();
        qDebug() << outputAudio->state();
    } else {
        qDebug() << inputAudio->state();
        qDebug() << outputAudio->state();
    }

    return running;
}

// returns the audio level for each channel
QVector<qreal> getBufferLevels(const QAudioBuffer& buffer)
{
    QVector<qreal> values;
    /*
    if (!buffer.format().isValid() || buffer.format().byteOrder() != QAudioFormat::LittleEndian)
        return values;

    if (buffer.format().codec() != "audio/pcm")
        return values;
    */
    int channelCount = buffer.format().channelCount();
    values.fill(0, channelCount);

    qreal peak_value = INT_MAX;
    if (qFuzzyCompare(peak_value, qreal(0)))
        return values;

    switch (buffer.format().sampleType()) {
    case QAudioFormat::Unknown:
    case QAudioFormat::UnSignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<quint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<quint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<quint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] = qAbs(values.at(i) - peak_value / 2) / (peak_value / 2);
        break;
    case QAudioFormat::Float:
        if (buffer.format().sampleSize() == 32) {
            values = getBufferLevels(buffer.constData<float>(), buffer.frameCount(), channelCount);
            for (int i = 0; i < values.size(); ++i)
                values[i] /= peak_value;
        }
        break;
    case QAudioFormat::SignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<qint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<qint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<qint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] /= peak_value;
        break;
    }

    return values;
}

template <class T>
QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels)
{
    QVector<qreal> max_values;
    max_values.fill(0, channels);

    for (int i = 0; i < frames; ++i) {
        for (int j = 0; j < channels; ++j) {
            qreal value = qAbs(qreal(buffer[i * channels + j]));
            if (value > max_values.at(j))
                max_values.replace(j, value);
        }
    }

    return max_values;
}
