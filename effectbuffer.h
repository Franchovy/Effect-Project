#ifndef EFFECTBUFFER_H
#define EFFECTBUFFER_H

#include <QObject>
#include <QIODevice>
#include <QDebug>
#include <QtMath>
#include <QAudioBuffer>


#include <effectsLib/echoeffect1.h>


class EffectBuffer : public QIODevice
{
public:
    EffectBuffer();
    Q_OBJECT

    qint64 readData(char* data, qint64 maxlen) override;
    qint64 writeData(const char* data, qint64 maxlen) override;

public:
    QList<Effect*>* getEffectChain() {return &effectChain;}
    void removeEffect(Effect* e);

private:
    QByteArray buffer; //TODO investigate using QAudioBuffer instead. Same for effectBuffer

    qint64 bufferUsed;
    qint64 bufferCurrent;

    QList<Effect*> effectChain;

    EchoEffect1 echoeffect;

    void applyEffect(char* in, char* out, int readLength);

public slots:
    void addEffect(Effect* e);

};

#endif // EFFECTBUFFER_H
