#ifndef AUDIO_H
#define AUDIO_H

#include "effectbuffer.h"

#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioOutput>
#include <QAudio>




class Audio : public QObject
{
public:
    Audio();
    Q_OBJECT

public:
    QList<Effect*>* getEffectChain(){return buffer->getEffectChain();}
    void addEffect(Effect* e);
    void removeEffect(Effect* e);

    EffectBuffer* getEffectBuffer() {return buffer;}

    QList<QAudioDeviceInfo> availableAudioInputDevices();
    QList<QAudioDeviceInfo> availableAudioOutputDevices();
    void setInputDevice(QAudioDeviceInfo device){inputDevice = device;}
    void setOutputDevice(QAudioDeviceInfo device){outputDevice = device;}


private:
    EffectBuffer* buffer;
    QAudioDeviceInfo inputDevice;
    QAudioDeviceInfo outputDevice;
    QAudioInput* inputAudio;
    QAudioOutput* outputAudio;

    bool running = false;

public slots:
    bool runAudio();

};

#endif // AUDIO_H
