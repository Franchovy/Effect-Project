#ifndef EFFECTBUFFER_H
#define EFFECTBUFFER_H

#include <QObject>
#include <QIODevice>
#include <QDebug>
#include <QtMath>
#include <QAudioBuffer>


#include <effectsLib/echoeffect1.h>

//Unused

struct AudioArrayElement{
    int val;
    int left;
    int right;
};


class AudioArray{
    char* buffer;
    int length;


    int sampleSizeBytes = 2;
    bool stereo = true;
    //TODO expand these
    //bool littleEndian = true;
    //bool signedInt = true;

    AudioArray(char* buffer, int bufferLength, QAudioFormat format){
        if (format.sampleSize() == 8) sampleSizeBytes = 1;
        else if (format.sampleSize() == 16) sampleSizeBytes = 2;
        else if (format.sampleSize() == 24) sampleSizeBytes = 2; //Weirdness

        if (format.channelCount() == 2) stereo = true;
        else stereo = false;

        length = bufferLength / sampleSizeBytes / format.channelCount();
        this->buffer = buffer;
    }

    AudioArrayElement operator[] (int index){
        AudioArrayElement e;
        if (stereo){
            //STEREO
            e.val = -1;

            if (sampleSizeBytes == 2){
                e.left = ((buffer[index*2] << 8) | ((0x00ff) & buffer[index*2+1]));
                e.right = ((buffer[index*2+2] << 8) | ((0x00ff) & buffer[index*2+3]));
            } else if (sampleSizeBytes == 1) {
                e.left = buffer[index];
                e.right = buffer[index+1];
            }
        } else {
            //MONO
            e.left = -1;
            e.right = -1;

            if (sampleSizeBytes == 2){
                e.val = ((buffer[index*2] << 8) | ((0x00ff) & buffer[index*2+1]));
            } else if (sampleSizeBytes == 1) {
                e.val = buffer[index];
            }
        }
        return e;
    }
};

class EffectBuffer : public QIODevice
{
public:
    EffectBuffer();
    Q_OBJECT

    qint64 readData(char* data, qint64 maxlen) override;
    qint64 writeData(const char* data, qint64 maxlen) override;

public: QList<Effect*>* getEffectChain() {return &effectChain;}

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
