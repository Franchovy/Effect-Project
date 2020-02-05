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
#include "effectsLib/delayeffect.h"
#include "ports/inport.h"
#include "ports/outport.h"


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

    audioSystemSet = false;
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
    case 7: // DelayEffect
        e = new DelayEffect(this);
        break;
    default:
        e = new Effect(this);
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

QAudioDeviceInfo Audio::getDefaultInputDevice()
{
    if (!inputDevice){
        return QAudioDeviceInfo::defaultInputDevice();
    } else return *inputDevice;
}

QAudioDeviceInfo Audio::getDefaultOutputDevice()
{
    if (!outputDevice){
        return QAudioDeviceInfo::defaultOutputDevice();
    } else return *outputDevice;
}

void Audio::setupFormat(AudioSystem audioSystem)
{
    format.setSampleRate(96000);
    format.setChannelCount(2);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    if (audioSystem == QT){
        format.setSampleType(QAudioFormat::SignedInt);
    } else if (audioSystem == JACK){
        format.setSampleType(QAudioFormat::Float);
    }



    format = inputDevice->nearestFormat(format);
    format = outputDevice->nearestFormat(format);

    inputAudio = new QAudioInput(*inputDevice, format, this);
    outputAudio = new QAudioOutput(*outputDevice, format, this);
    Effect::setAudioFormat(format);

    currentAudioSystem = audioSystem;
}

void Audio::setInputDevice(QAudioDeviceInfo device)
{
    inputDevice = new QAudioDeviceInfo(device);
}

void Audio::setOutputDevice(QAudioDeviceInfo device)
{
    outputDevice = new QAudioDeviceInfo(device);
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

void Audio::setAudioSystem(AudioSystem audioSystem)
{
    qDebug() << "Set audio system: " << audioSystem;
    if (running)
        stopAudio();
    setupFormat(audioSystem);
    audioSystemSet = true;
}

bool Audio::runAudio()
{    
    if (!running) {

        qDebug() << "Input Device: " << inputDevice->deviceName();
        qDebug() << "Output Device: " << outputDevice->deviceName();
        qDebug() << "Audio Running!";
        running = true;

        if (currentAudioSystem == QT){
            m_buffer->open(QIODevice::ReadWrite);

            inputAudio->start(m_buffer);
            outputAudio->start(m_buffer);
        } else if (currentAudioSystem == JACK) {
            running = m_buffer->runJackAudio();
        } else {
            qDebug() << "Uhhhh, audio system not set.";
        }
    }

    return running;
}

void Audio::stopAudio()
{
    if (currentAudioSystem == QT){
        m_buffer->close();
        inputAudio->stop();
        outputAudio->stop();
    } else if (currentAudioSystem == JACK){
        m_buffer->stopJackAudio();
    }

    running = false;
}
