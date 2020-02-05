#include "delayeffect.h"

#include <QDebug>

DelayEffect::DelayEffect(Audio* parent) : Effect(parent)
{
    type = 7;
    effectName = "Delay Effect";

    in = new InPort("Delay In", this);
    out = new OutPort("Delay out", this);
    addPort(in);
    addPort(out);

    buffer = new char[delayAmount];
    bufferLength = delayAmount;
    bufferpt = 0;
}

void DelayEffect::applyEffect(char *in, char *out, int readLength)
{
    //qDebug() << "Bufferpt: " << bufferpt;
    //qDebug() << "Readlength: " << readLength;
    if (start){
        if (bufferpt + readLength < bufferLength){
            //qDebug() << "start";
            memcpy(&buffer[bufferpt], in, readLength);
            for (int i = 0; i < readLength; i++){
                out[i] = 0;
            }
            bufferpt += readLength;
        } else {
            start = false;
            //qDebug() << "recursing";
            applyEffect(in, out, readLength);
        }
    } else {
        if (readLength + bufferpt > bufferLength){
            //circular
            //qDebug() << "Circular";
            //qDebug() << "BufferLength-bufferpt="<<bufferLength-bufferpt;
            // Copy end of buffer
            memcpy(out, &buffer[bufferpt], bufferLength-bufferpt);
            memcpy(&buffer[bufferpt], in, bufferLength-bufferpt);
            // Copy front of buffer
            //qDebug() << "readLength - (bufferLength-bufferpt)=" << readLength - (bufferLength-bufferpt);
            memcpy(&out[bufferLength-bufferpt], buffer, readLength - (bufferLength-bufferpt));
            memcpy(buffer, &in[bufferLength-bufferpt], readLength - (bufferLength-bufferpt));
            bufferpt = readLength - bufferLength + bufferpt;
        } else {
            //straightforward
            //qDebug() << "Straightforward";
            memcpy(out, &buffer[bufferpt], readLength);
            memcpy(&buffer[bufferpt+readLength], in, readLength);
            bufferpt += readLength;
        }
    }
    //qDebug() << "Bufferpt end:" << bufferpt;
}

char *DelayEffect::getData(OutPort *, int readLength)
{
    char* data = getOutPortData(outPorts.first(),readLength);
    if (bufferLength < readLength){
        qDebug() << "Extending buffer";
        //resize if buffer is too short
        char* newbuffer = new char[readLength];
        memcpy(newbuffer, buffer, bufferLength);
        bufferLength = readLength;
        buffer = newbuffer;
    }
    DelayEffect::applyEffect(inPorts.first()->getData(), data, readLength);
    return data;
}

