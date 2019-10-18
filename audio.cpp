#include "audio.h"

Audio::Audio()
{
    buffer = new EffectBuffer();

    //Select audio device - no hassle mode.
    QList<QAudioDeviceInfo> inDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    QList<QAudioDeviceInfo> outDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);

    // Default in/out sound device select.
    int inSelect = 0;
    int outSelect = 0;

    //qDebug() << "Input Devices list: ";
    for (int i = 0; i < inDevices.length(); i++){
        //qDebug() << inDevices.at(i).deviceName();
        // test with pulse
        if (inDevices.at(i).deviceName() == "alsa_input.usb-0d8c_C-Media_USB_Headphone_Set-00.analog-mono") inSelect = i;
        //alsa_input.usb-0d8c_C-Media_USB_Headphone_Set-00.analog-mono
        //"alsa_input.pci-0000_00_1f.3.analog-stereo"
    }
    //qDebug() << "Output Devices list: ";
    for (int i = 0; i < outDevices.length(); i++){
        //qDebug() << outDevices.at(i).deviceName();
        // test with pulse
        if (outDevices.at(i).deviceName() == "alsa_output.pci-0000_00_1f.3.analog-stereo") outSelect = i;
    }


    QAudioDeviceInfo inputDevice = QAudioDeviceInfo(inDevices.at(inSelect));
    QAudioDeviceInfo outputDevice = QAudioDeviceInfo(outDevices.at(outSelect));


    //Select default audio device. Even less hassle mode.
    /*
    QAudioDeviceInfo inputDevice = QAudioDeviceInfo::defaultInputDevice();
    QAudioDeviceInfo outputDevice = QAudioDeviceInfo::defaultOutputDevice();
    */
    qDebug() << "Selected input: " << inputDevice.deviceName();
    qDebug() << "Selected output: " << outputDevice.deviceName();

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
    //inputAudio = new QAudioInput(format);   <--  this works.
    //outputAudio = new QAudioOutput(format);

    qDebug() << "Audio initialised.";
}

void Audio::runAudio()
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
}
