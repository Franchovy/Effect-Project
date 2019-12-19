#ifndef AUDIO_H
#define AUDIO_H

#include "effectsscene.h"

#include <QObject>

QT_BEGIN_NAMESPACE
class Effect;
class EffectBuffer;
class EffectMap;
class EffectsScene;
class InPort;
class InputEffect;
class OutPort;
class OutputEffect;
class QAudioInput;
class QAudioOutput;
class QAudioDeviceInfo;
QT_END_NAMESPACE


class Audio : public QObject
{
public:
    Audio(QObject* parent = nullptr);
    Q_OBJECT

public:
    Effect* createEffect(int effectType);
    void addEffect(Effect* e);
    void removeEffect(Effect* e);

    EffectMap* getEffectMap() {return m_effectMap;}
    EffectBuffer* getEffectBuffer() {return m_buffer;}

    QList<QAudioDeviceInfo> availableAudioInputDevices();
    QList<QAudioDeviceInfo> availableAudioOutputDevices();
    void setInputDevice(QAudioDeviceInfo device);
    void setOutputDevice(QAudioDeviceInfo device);

    EffectsScene* getUI();
    void setUI(EffectsScene* ui);

private:
    EffectsScene* UI;

    EffectBuffer* m_buffer;
    EffectMap* m_effectMap;

    QAudioDeviceInfo* inputDevice;
    QAudioDeviceInfo* outputDevice;
    QAudioInput* inputAudio;
    QAudioOutput* outputAudio;

    bool running = false;

    void setupFormat();

public slots:
    bool runAudio();

};

#endif // AUDIO_H
