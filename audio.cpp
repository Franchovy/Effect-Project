#include "audio.h"

#include <QDebug>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QAudio>

#include "effect.h"
#include "effectmap.h"
#include "effectbuffer.h"
#include "effectsLib/inputeffect.h"
#include "effectsLib/outputeffect.h"
#include "ports/inport.h"
#include "ports/outport.h"


Audio::Audio(QObject* parent) :
    QObject(parent),
    m_buffer(new EffectBuffer(this)),
    m_effectMap(new EffectMap(this))
{
    //Old implementation
    //Set connection
    //setConnectedPort(m_inEffect->inputDevicePort, m_outEffect->outputDevicePort);

    inputDevice = new QAudioDeviceInfo(QAudioDeviceInfo::defaultInputDevice());
    outputDevice = new QAudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());


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
    format.setSampleSize(24);
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
