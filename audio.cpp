#include "audio.h"

#include "effect.h"
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
    inEffect(new InputEffect(this)),
    outEffect(new OutputEffect(this)),
    buffer(new EffectBuffer(inEffect, outEffect))
{
    //Set connection
    inEffect->setConnectedPort(inEffect->inputDevicePort, outEffect->outputDevicePort);


    inputDevice = new QAudioDeviceInfo(QAudioDeviceInfo::defaultInputDevice());
    outputDevice = new QAudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());

    setupFormat();
}

QList<Effect *> *Audio::getEffectChain()
{
    return buffer->getEffectChain();
}

void Audio::addEffect(Effect *e)
{
    buffer->addEffect(e);
}

void Audio::removeEffect(Effect* e)
{
    buffer->removeEffect(e);
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

        buffer->open(QIODevice::ReadWrite);

        //outputAudio->start(inputAudio->start());

        inputAudio->start(buffer);
        outputAudio->start(buffer);

        qDebug() << inputAudio->state();
        qDebug() << outputAudio->state();
    } else {
        qDebug() << inputAudio->state();
        qDebug() << outputAudio->state();
    }

    return running;
}
