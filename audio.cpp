#include "audio.h"

#include "effect.h"
#include "effectmap.h"
#include "effectbuffer.h"
#include "effectsLib/inputeffect.h"
#include "effectsLib/outputeffect.h"
#include "ports/inport.h"
#include "ports/outport.h"
#include <QDebug>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QAudio>


Audio::Audio(QObject* parent) :
    QObject(parent),
    m_inEffect(new InputEffect(this)),
    m_outEffect(new OutputEffect(this)),
    m_effectMap(new EffectMap(this)),
    m_buffer(new EffectBuffer(m_inEffect, m_outEffect))
{
    //Old implementation
    //Set connection
    //setConnectedPort(m_inEffect->inputDevicePort, m_outEffect->outputDevicePort);

    //New implementation
    m_effectMap->addEffect(m_inEffect);
    m_effectMap->addPort(m_inEffect, m_inEffect->inputDevicePort);

    m_effectMap->addEffect(m_outEffect);
    m_effectMap->addPort(m_outEffect, m_outEffect->outputDevicePort);

    m_effectMap->connectPorts(m_inEffect->inputDevicePort, m_outEffect->outputDevicePort);

    inputDevice = new QAudioDeviceInfo(QAudioDeviceInfo::defaultInputDevice());
    outputDevice = new QAudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());

    setupFormat();
}

QList<Effect *> *Audio::getEffectMap()
{
    return m_buffer->getEffectChain();
}

void Audio::addEffect(Effect *e)
{
    m_effectMap->addEffect(e);
    /*
    Port* p;
    foreach(p, e.getPorts()){
        m_effectMap->addPort(p);
    }
    */
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
