#include "delayeffect.h"

DelayEffect::DelayEffect(Audio* parent) : Effect(parent)
{
    type = 7;
    effectName = "Delay Effect";

    in = new InPort("Delay In", this);
    out = new OutPort("Delay out", this);
    addPort(in);
    addPort(out);

    delayCounter = delayAmount;
}

void DelayEffect::applyEffect(char *in, char *out, int readLength)
{

}

char *DelayEffect::getData(OutPort *, int readLength)
{
    char* data = getOutPortData(outPorts.first(),readLength);
    if (bufferLength < bufferpt + readLength){
        //TODO
    }

    memcpy(buffer, data, readLength);
}

