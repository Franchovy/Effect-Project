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
class QAudioBuffer;
class QAudioInput;
class QAudioOutput;
class QAudioDeviceInfo;
class QAudioProbe;
class QAudioRecorder;
QT_END_NAMESPACE


class Audio : public QObject
{
    Q_OBJECT
public:
    Audio(QObject* parent = nullptr);

    EffectMap* getEffectMap() {return m_effectMap;}
    EffectBuffer* getEffectBuffer() {return m_buffer;}

    QList<QAudioDeviceInfo> availableAudioInputDevices();
    QList<QAudioDeviceInfo> availableAudioOutputDevices();
    void setInputDevice(QAudioDeviceInfo device);
    void setOutputDevice(QAudioDeviceInfo device);

    void record();

    bool isRunning() {return running;}

private:
    EffectBuffer* m_buffer;
    EffectMap* m_effectMap;

    // Temp
    QAudioInput* inputAudio;
    QAudioOutput* outputAudio;
    QAudioDeviceInfo* inputDevice;
    QAudioDeviceInfo* outputDevice;

    QAudioRecorder* m_audioRecorder;
    QAudioProbe* m_audioProbe;

    int audioSystem = 1; // 0 - QIODevice, 1 - JACK

    QVector<qreal> getBufferLevels(const QAudioBuffer& buffer);

    template <class T>
    QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels);

    bool running = false;

    void setupFormat();

signals:
    void newEffectSignal(Effect* e);
    // For use in EffectMap
    void deleteEffectSignal(Effect* e);
    void connectPortsSignal(Port*, Port*);
    void disconnectPortsSignal(Port*, Port*);

    void runAudioSignal();
    void stopAudioSignal();


public slots:
    void createEffect(int effectType);
    void deleteEffect(Effect*);
    void connectPorts(QPair<Effect*,int>,QPair<Effect*,int>);
    void disconnectPorts(QPair<Effect*,int>,QPair<Effect*,int>);

    bool runAudio();
    void stopAudio();
    //void processBuffer(const QAudioBuffer&);

};

#endif // AUDIO_H
