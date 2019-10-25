#include "effectbuffer.h"


//TODO sort out conversions!!!
//TODO sort out multiple instances. Including switching streams etc.
EffectBuffer::EffectBuffer()
{
    buffer = QByteArray();

    effectChain = QList<Effect*>();

    //EchoEffect1* echoeffect = new EchoEffect1();
    //effectChain.append(echoeffect);
}

qint64 EffectBuffer::readData(char* data, qint64 maxlen){
    Q_ASSERT(static_cast<int>(maxlen) == maxlen); //check that conversion isn't fucking things up
    int readLength = qMin(static_cast<int>(maxlen), buffer.length());

    applyEffect(buffer.data(), data, readLength);

    buffer.remove(0, readLength);

    return readLength;
}

qint64 EffectBuffer::writeData(const char* data, qint64 maxlen){
    if(maxlen > (buffer.capacity() + buffer.size())){
        //  Increase buffer capacity to new maximum.
        qint64 newCap = buffer.size() + maxlen;
        //qDebug() << "Extending size to: " << newCap;
        Q_ASSERT(static_cast<int>(newCap) == newCap);
        buffer.reserve(static_cast<int>(newCap)); //check that conversion isn't fucking things up

    }

    Q_ASSERT(static_cast<int>(maxlen) == maxlen); //check that conversion isn't fucking things up
    buffer.append(data, static_cast<int>(maxlen));
    return maxlen;
}

void EffectBuffer::applyEffect(char* in, char* out, int readLength){
    for (Effect* e : effectChain){
        qDebug() << "Applying: " << e->effectName;
        e->applyEffect(in, out, readLength);
        memcpy(in,out,readLength);
    }
}

void EffectBuffer::addEffect(Effect *e)
{
    effectChain.append(e);
}
