#ifndef EFFECTBUFFER_H
#define EFFECTBUFFER_H

#include <QObject>
#include <QIODevice>
#include <QDebug>
#include <QtMath>
#include <QAudioBuffer>

class EffectBuffer : public QIODevice
{
public:
    EffectBuffer();
    Q_OBJECT

    qint64 readData(char* data, qint64 maxlen) override;
    qint64 writeData(const char* data, qint64 maxlen) override;


private:
    QByteArray buffer; //TODO investigate using QAudioBuffer instead. Same for effectBuffer

    qint64 bufferUsed;
    qint64 bufferCurrent;


    qint64 validElementStart;
    qint64 validElementEnd;

    QByteArray effectBuffer;
    int effectBufferpt;

    void initEffect(QByteArray effectBuffer, qint64 length);
    void applyEffect(char* in, char* out, int readLength);

/*
public slots:
    void applyEffects(qint64 bytes);
//    void readyRead();
*/
signals:
    void bytesWritten(qint64 bytes);

};

#endif // EFFECTBUFFER_H
