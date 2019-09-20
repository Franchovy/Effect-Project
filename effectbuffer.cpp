#include "effectbuffer.h"



//TODO sort out conversions!!!
//TODO sort out multiple instances. Including switching streams etc.
EffectBuffer::EffectBuffer()
{
    buffer = QByteArray();

    initEffect(effectBuffer, 1000000);

    validElementStart = 0;
    validElementEnd = 0;

    //connect(this, &EffectBuffer::bytesWritten, this, &EffectBuffer::applyEffects);
}


qint64 EffectBuffer::readData(char* data, qint64 maxlen){
    int readLength = qMin(static_cast<int>(maxlen), buffer.length());
    //qDebug() << "Maxlen: " << maxlen << ", readLength: " << readLength;
    //  can chopped data be const? linear time -> constant time (?)

    applyEffect(buffer.data(), data, readLength);

    //memcpy(data, buffer.data(), static_cast<size_t>(readLength));
    buffer.remove(0, readLength);

    return readLength;
}

qint64 EffectBuffer::writeData(const char* data, qint64 maxlen){
    if(maxlen > (buffer.capacity() + buffer.size())){
        //  Increase buffer capacity to new maximum.
        qint64 newCap = buffer.size() + maxlen;
        //qDebug() << "Extending size to: " << newCap;
        buffer.reserve(newCap);
    }

    buffer.append(data, static_cast<int>(maxlen));
    return maxlen;
}

void EffectBuffer::initEffect(QByteArray effectBuffer, qint64 length)
{
    effectBuffer = QByteArray(static_cast<int>(length), 0x01);
    effectBufferpt = 0;
}

void EffectBuffer::applyEffect(char* in, char* out, int readLength){
    //Apply effects here:

    if (effectBuffer.length() < 1000000) {
        effectBuffer.resize(1000000);
        effectBuffer.fill(0);
    }

    for (int i = 0; i < readLength; i++){
        //out[i] = in->at(i);

        effectBuffer[effectBufferpt] = in[i];

        effectBufferpt++;
        if (effectBufferpt >= effectBuffer.length()) {
            effectBufferpt = 0;
        }
        out[i] = in[i] + effectBuffer.at(effectBufferpt);
    }
}

/*
void EffectBuffer::applyEffects(qint64 bytes){
    int applied = validElementEnd - bytes;
    //CHANGEME loopback compatibility
    //loop through bytes
    while (applied <= bytes){
        //loop through effects
        buffer2[applied] = buffer2.at(applied); //???? same buffer in/out??
        qDebug() << buffer2[applied];

        applied++;
    }
}
*/
