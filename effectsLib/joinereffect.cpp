#include "joinereffect.h"

JoinerEffect::JoinerEffect(Audio *parent) : Effect(parent)
{
    effectName = "Joiner Effect";
    type = 5;

    in1 = new InPort("Joiner In 1", this);
    in2 = new InPort("Joiner In 2", this);
    out = new OutPort("Joiner Out", this);

    addPort(in1,QPointF(150,100));
    addPort(in2, QPointF(150, 50));
    addPort(out, QPointF(50, 100));

}

void JoinerEffect::applyEffect(char *in, char *out, int readLength)
{
    for (int i = 0; i < readLength; i += step){

    }
    addMode ^= addMode; // switch addMode
}

char *JoinerEffect::getData(OutPort *, int readLength)
{
    // Only one outport
    char* outData = outPorts.first()->data;
    if (outPorts.first()->dataLength < readLength){
        // Initialise data container in output port
        outData = new char[readLength];
        // Copy over any data needed
        memcpy(outData, outPorts.first()->data, outPorts.first()->dataLength);
        // Set new Outport data
        outPorts.first()->data = outData;
        outPorts.first()->dataLength = readLength;
    }
    JoinerEffect::applyEffect(inPorts.at(0)->getData(), outPorts.first()->data, readLength);
    JoinerEffect::applyEffect(inPorts.at(1)->getData(), outPorts.first()->data, readLength);
    return outData;
}
