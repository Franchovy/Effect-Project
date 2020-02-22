#include "stereojoinereffect.h"

#include <effectmap.h>

StereoJoinerEffect::StereoJoinerEffect(Audio* parent) : Effect(parent)
{
    effectName = "Stereo Joiner";
    type = 8;

    in1 = new InPort("StereoJoiner In 1", this);
    in2 = new InPort("StereoJoiner In 2", this);
    out = new OutPort("StereoJoiner Out", this);
    addPort(in1);
    addPort(in2);
    addPort(out);
}

void StereoJoinerEffect::applyEffect(char *in1, char* in2, char *out, int readLength)
{
    for (int i = 0; i < readLength; i += step){
        memcpy(&out[i], &in1[i], step);
        memcpy(&out[i+step], &in2[i], step);
    }
}

char *StereoJoinerEffect::getData(OutPort *, int readLength)
{
    char* outData = getOutPortData(outPorts.first(),readLength);
    effectMap->readLength = readLength/2; //WARNING make sure this is right
    applyEffect(in1->getData(), in2->getData(), outData, readLength);
    return outData;
}
