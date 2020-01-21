#ifndef EFFECTBUFFER_H
#define EFFECTBUFFER_H

#include <QObject>
#include <QIODevice>

QT_BEGIN_NAMESPACE
class Audio;
class Effect;
class EffectMap;
class InPort;
class InputEffect;
class OutPort;
class OutputEffect;
class QtMath;
class QDebug;
class QAudioBuffer;
QT_END_NAMESPACE

class EffectBuffer : public QIODevice
{
    Q_OBJECT

public:
    EffectBuffer(Audio *parent);

    qint64 readData(char* data, qint64 maxlen) override;
    qint64 writeData(const char* data, qint64 maxlen) override;

    void setEffectMap(EffectMap*);

    void addInputEffect(InputEffect* e);
    void addOutputEffect(OutputEffect* e);

    void deleteInputEffect(InputEffect* e);
    void deleteOutputEffect(OutputEffect* e);

    QList<InputEffect *> getInputEffects() const;
    QList<OutputEffect *> getOutputEffects() const;

    EffectMap *getEffectMap() const;

    int runJackAudio();
    void stopJackAudio();

private:
    QByteArray buffer; //TODO investigate using QAudioBuffer instead. Same for effectBuffer

    EffectMap* effectMap;

    qint64 bufferUsed;
    qint64 bufferCurrent;

    //New implementation
    QList<InputEffect*> inputEffects;
    QList<OutputEffect*> outputEffects;

    void applyEffect(char* in, char* out, int readLength);
};

#endif // EFFECTBUFFER_H
