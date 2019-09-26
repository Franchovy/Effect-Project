#include "effectbuffer.h"


//TODO sort out conversions!!!
//TODO sort out multiple instances. Including switching streams etc.
EffectBuffer::EffectBuffer()
{
    buffer = QByteArray();

    effectChain = QList<Effect*>();

    EchoEffect1* echoeffect = new EchoEffect1();
    effectChain.append(echoeffect);
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

void EffectBuffer::applyEffect(char* in, char* out, int readLength){
    //Apply effects here:

    for (Effect* e : effectChain){
        e->applyEffect(in, out, readLength);
    }
}

void EffectBuffer::addEffect(Effect *e)
{
    effectChain.append(e);
}
