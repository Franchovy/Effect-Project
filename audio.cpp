#include "audio.h"

#include <QDebug>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
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
#include "effectsLib/echoeffect1.h"
#include "effectsLib/paneffect.h"
#include "effectsLib/waveeffect.h"
#include "effectsLib/joinereffect.h"
#include "effectsLib/splittereffect.h"
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
    // Set up recording

    inputDevice = new QAudioDeviceInfo(QAudioDeviceInfo::defaultInputDevice());
    outputDevice = new QAudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());

    m_audioProbe->setSource(m_audioRecorder);

    m_audioRecorder = new QAudioRecorder(this);
    m_audioProbe = new QAudioProbe(this);

    // Make connections with EffectMap
    connect(this, &Audio::newEffectSignal, m_effectMap, &EffectMap::addEffect);
    connect(this, &Audio::deleteEffectSignal, m_effectMap, &EffectMap::deleteEffect);
    connect(this, &Audio::connectPortsSignal, m_effectMap, &EffectMap::connectPorts);
    connect(this, &Audio::disconnectPortsSignal, m_effectMap, &EffectMap::disconnectPorts);

    m_buffer->setEffectMap(m_effectMap);

    setupFormat();
}

//Upon receiving signal.
void Audio::createEffect(int effectType)
{
    Effect* e;
    switch (effectType){
    case 0: //InputEffect
        e = new InputEffect(this);
        m_buffer->addInputEffect(static_cast<InputEffect*>(e));
        break;
    case 1: //OutputEffect
        e = new OutputEffect(this);
        m_buffer->addOutputEffect(static_cast<OutputEffect*>(e));
        break;
    case 2: // EchoEffect1
        e = new EchoEffect1(this);
        break;
    case 3: // PanEffect
        e = new PanEffect(this);
        break;
    case 4: // WaveEffect
        e = new WaveEffect(this);
        break;
    case 5: // JoinerEffect
        e = new JoinerEffect(this);
        break;
    case 6: // SplitterEffect
        e = new SplitterEffect(this);
        break;
    default:
        qDebug() << "Unknown Effect requested.";
    }
    newEffectSignal(e);
}

void Audio::deleteEffect(Effect *e)
{
    // Pass down to EffectMap
    m_effectMap->deleteEffect(e);

    if (e->type == 0){
        m_buffer->deleteInputEffect(static_cast<InputEffect*>(e));
    } else if (e->type == 1){
        m_buffer->deleteOutputEffect(static_cast<OutputEffect*>(e));
    }
}

void Audio::connectPorts(QPair<Effect *, int> e1, QPair<Effect *, int> e2)
{
    // TODO
    connectPortsSignal(e1.first->getPorts().at(e1.second), e2.first->getPorts().at(e2.second));
}

void Audio::disconnectPorts(QPair<Effect *, int> e1, QPair<Effect *, int> e2)
{
    // TODO
    disconnectPortsSignal(e1.first->getPorts().at(e1.second), e2.first->getPorts().at(e2.second));
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
    format.setSampleRate(96000);
    format.setChannelCount(2);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    if (audioSystem == Qt){
        qDebug() << "Audio system set to Qt";
        format.setSampleType(QAudioFormat::SignedInt);
    } else if (audioSystem == JACK){
        qDebug() << "Audio system set to JACK";
        format.setSampleType(QAudioFormat::Float);
    }

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
            //CHANGEME
            container = n;
        }

        QAudioEncoderSettings settings;

        for (auto a : m_audioRecorder->supportedAudioCodecs()){
            //CHANGEME
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

bool Audio::runAudio()
{
    if (!running) {
        qDebug() << "Input Device: " << inputDevice->deviceName();
        qDebug() << "Output Device: " << outputDevice->deviceName();

        qDebug() << "Audio Running!";
        running = true;

        if (audioSystem == Qt){
            m_buffer->open(QIODevice::ReadWrite);

            inputAudio->start(m_buffer);
            outputAudio->start(m_buffer);
        } else if (audioSystem == JACK) {
            m_buffer->runJackAudio();
        }

        qDebug() << inputAudio->state();
        qDebug() << outputAudio->state();
    }

    return running;
}

void Audio::stopAudio()
{
    if (audioSystem == Qt){
        m_buffer->close();
        inputAudio->stop();
        outputAudio->stop();
    } else if (audioSystem == JACK){
        m_buffer->stopJackAudio();
    }

    running = false;
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
