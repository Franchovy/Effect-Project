#include "splittereffect.h"

SplitterEffect::SplitterEffect(Audio* parent) : Effect(parent)
{
    effectName = "Splitter Effect";
    type = 6;

    in = new InPort("Splitter In", this);
    out1 = new OutPort("Splitter Out 1", this);
    out2 = new OutPort("Splitter Out 2", this);
    addPort(in);
    addPort(out1);
    addPort(out2);
}

void SplitterEffect::applyEffect(char *in, char *out, int readLength)
{

}

char *SplitterEffect::getData(OutPort *port, int readLength)
{
    char* outData = getOutPortData(port, readLength);
    memcpy(outData, inPorts.first()->getData(), readLength);

    return outData;
}
