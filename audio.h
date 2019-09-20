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

private:
    EffectBuffer* buffer;
    QAudioInput* inputAudio;
    QAudioOutput* outputAudio;

    bool running = false;

public slots:
    void runAudio();

};

#endif // AUDIO_H
