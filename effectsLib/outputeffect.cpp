#include "outputeffect.h"

#include "ports/inport.h"

OutputEffect::OutputEffect(QObject *parent) : Effect(parent)
{
    outputDevicePort = new InPort("Output device port", this);
}

void OutputEffect::applyEffect(char *in, char *out, int readLength)
{
    out = in;
}

char *OutputEffect::getData()
{
    return outputDevicePort->getData();
}
