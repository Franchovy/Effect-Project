#ifndef AUDIO_H
#define AUDIO_H

#include "effectsscene.h"

#include <QObject>
#include <QAudioFormat>

QT_BEGIN_NAMESPACE
class Effect;
class EffectBuffer;
class EffectMap;
class EffectsScene;
class InPort;
class InputEffect;
class OutPort;
class OutputEffect;
class QAudioBuffer;
class QAudioInput;
class QAudioOutput;
class QAudioDeviceInfo;
class QAudioFormat;
class QAudioProbe;
class QAudioRecorder;
QT_END_NAMESPACE

enum AudioSystem {QT, JACK};

class Audio : public QObject
{
    Q_OBJECT
public:
    Audio(QObject* parent = nullptr);

    EffectMap* getEffectMap() {return m_effectMap;}
    EffectBuffer* getEffectBuffer() {return m_buffer;}

    QList<QAudioDeviceInfo> availableAudioInputDevices();
    QList<QAudioDeviceInfo> availableAudioOutputDevices();
    QAudioDeviceInfo getDefaultInputDevice();
    QAudioDeviceInfo getDefaultOutputDevice();
    void setInputDevice(QAudioDeviceInfo device);
    void setOutputDevice(QAudioDeviceInfo device);

    QAudioFormat getAudioFormat(){return format;}

    void record();
    bool isRunning() {return running;}
    int baseEffectsCount(){return 8;}

    void setAudioSystem(AudioSystem audioSystem);
    bool isAudioSystemSet(){return audioSystemSet;}

private:
    EffectBuffer* m_buffer;
    EffectMap* m_effectMap;

    QAudioFormat format;
    bool audioSystemSet;
    AudioSystem currentAudioSystem;

    // Temp
    QAudioInput* inputAudio;
    QAudioOutput* outputAudio;
    QAudioDeviceInfo* inputDevice;
    QAudioDeviceInfo* outputDevice;

    QAudioRecorder* m_audioRecorder;
    QAudioProbe* m_audioProbe;

    bool running = false;

    void setupFormat(AudioSystem audioSystem);

Q_SIGNALS:
    void newEffectSignal(Effect* e);
    // For use in EffectMap
    void deleteEffectSignal(Effect* e);
    void connectPortsSignal(Port*, Port*);
    void disconnectPortsSignal(Port*, Port*);

    void runAudioSignal();
    void stopAudioSignal();


public Q_SLOTS:
    void createEffect(int effectType);
    void deleteEffect(Effect*);
    void connectPorts(QPair<Effect*,int>,QPair<Effect*,int>);
    void disconnectPorts(QPair<Effect*,int>,QPair<Effect*,int>);

    bool runAudio();
    void stopAudio();
    //void processBuffer(const QAudioBuffer&);

};

#endif // AUDIO_H
