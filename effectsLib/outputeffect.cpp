#include "outputeffect.h"

#include "ports/inport.h"

OutputEffect::OutputEffect(EffectMap* parent) : Effect(parent),
    outputDevicePort(new InPort("Output device port", this))
{
    effectName = "Output Device Name";

    m_inPortList.append(outputDevicePort);
}

void OutputEffect::applyEffect(char *in, char *out, int readLength)
{
    out = in;
}

char *OutputEffect::getData()
{
    return outputDevicePort->getData();
}
