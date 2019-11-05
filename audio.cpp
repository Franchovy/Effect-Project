#include "audio.h"

Audio::Audio()
{
    buffer = new EffectBuffer();

    inputDevice = QAudioDeviceInfo::defaultInputDevice();
    outputDevice = QAudioDeviceInfo::defaultOutputDevice();

    qDebug() << "Input Device: " << inputDevice.deviceName();
    qDebug() << "Output Device: " << outputDevice.deviceName();

    QAudioFormat format;
    format.setSampleRate(96000);
    format.setChannelCount(2);
    format.setSampleSize(24);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleSize(QAudioFormat::SignedInt);

    format = inputDevice.nearestFormat(format);
    format = outputDevice.nearestFormat(format);

    inputAudio = new QAudioInput(inputDevice, format, this);
    outputAudio = new QAudioOutput(outputDevice, format, this);
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

bool Audio::runAudio()
{
    if (!running) {
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
