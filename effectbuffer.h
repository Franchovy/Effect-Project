#ifndef EFFECTBUFFER_H
#define EFFECTBUFFER_H

#include <QObject>
#include <QIODevice>

QT_BEGIN_NAMESPACE
class Effect;
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
    EffectBuffer();

    qint64 readData(char* data, qint64 maxlen) override;
    qint64 writeData(const char* data, qint64 maxlen) override;

    void addInputEffect(InputEffect* e);
    void addOutputEffect(OutputEffect* e);

public:
    QList<Effect*>* getEffectChain() {return &effectChain;}
    void removeEffect(Effect* e);

private:
    QByteArray buffer; //TODO investigate using QAudioBuffer instead. Same for effectBuffer

    qint64 bufferUsed;
    qint64 bufferCurrent;

    //Old implementation
    QList<Effect*> effectChain;

    //New implementation
    QList<InputEffect*> inputEffects;
    QList<OutputEffect*> outputEffects;

    void applyEffect(char* in, char* out, int readLength);

public slots:
    void addEffect(Effect* e);

};

#endif // EFFECTBUFFER_H
