#include "effectbuffer.h"

#include "ports/inport.h"
#include "ports/outport.h"
#include "effectsLib/inputeffect.h"
#include "effectsLib/outputeffect.h"
#include "effect.h"
#include <QDebug>
#include <QtMath>
#include <QAudioBuffer>

//TODO sort out conversions!!!
//TODO sort out multiple instances. Including switching streams etc.
EffectBuffer::EffectBuffer()
{
    buffer = QByteArray();

    //effectChain = QList<Effect*>();

    //Use default input and output for now
}

//EffectBuffer (QIODevice) is being asked to read data from whatever. Aka - write to "data" (param)
qint64 EffectBuffer::readData(char* data, qint64 maxlen){
    Q_ASSERT(static_cast<int>(maxlen) == maxlen); //check that conversion isn't fucking things up
    int readLength = qMin(static_cast<int>(maxlen), buffer.length());

    //Supply data to inputEffects
    for (InputEffect* in_e : inputEffects){
        in_e->giveData(buffer.data());
    }
    //Output data from outputEffects
    for (OutputEffect* out_e : outputEffects){
        memcpy(data, out_e->getData(), readLength);
    }

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

void EffectBuffer::addInputEffect(InputEffect *e)
{
    inputEffects.append(e);
}

void EffectBuffer::addOutputEffect(OutputEffect *e)
{
    outputEffects.append(e);
}

void EffectBuffer::removeEffect(Effect *e)
{
    effectChain.removeOne(e);
}

/*
void EffectBuffer::applyEffect(char* in, char* out, int readLength){
    for (Effect* e : effectChain){
        e->applyEffect(in, out, readLength);
        memcpy(in,out,readLength);

    }
}
*/

void EffectBuffer::addEffect(Effect *e)
{
    effectChain.append(e);
}
