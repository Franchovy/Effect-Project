#include "outputeffect.h"

#include "ports/inport.h"

OutputEffect::OutputEffect(Audio* parent) : Effect(parent),
    outputDevicePort(new InPort("Output device port", this))
{
    effectName = "Output Device Name";

    ports.append(outputDevicePort);
}

void OutputEffect::applyEffect(char *in, char *out, int readLength)
{
    out = in;
}

char *OutputEffect::getData()
{
    //
}
