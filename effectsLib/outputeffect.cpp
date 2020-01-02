#include "outputeffect.h"

#include "ports/inport.h"

OutputEffect::OutputEffect(Audio* parent) : Effect(parent),
    outputDevicePort(new InPort("Output device port", this))
{
    effectName = "Output Device Name";
    type = 1;
    addPort(outputDevicePort, QPointF(150,100));
}

void OutputEffect::applyEffect(char *in, char *out, int readLength)
{
    out = in;
}

char *OutputEffect::getData(OutPort*, int readLength)
{
    //
}
