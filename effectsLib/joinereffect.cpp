#include "joinereffect.h"

#include <QDebug>
#include "effectmap.h"


JoinerEffect::JoinerEffect(Audio *parent) : Effect(parent)
{
    effectName = "Joiner Effect";
    type = 5;

    in1 = new InPort("Joiner In 1", this);
    in2 = new InPort("Joiner In 2", this);
    out = new OutPort("Joiner Out", this);

    addPort(in1,QPointF(150,125));
    addPort(in2, QPointF(150, 75));
    addPort(out, QPointF(50, 100));

}

void JoinerEffect::applyEffect(char *in1, char *in2, char *out, int readLength)
{
    for (int i = 0; i < readLength; i += step){
        if (format.sampleType() == QAudioFormat::SignedInt){
            int16_t x1;
            memcpy(&x1, &in1[i], step);
            int16_t x2;
            memcpy(&x2, &in2[i], step);
            x1 += x2;
            memcpy(&out[i], &x1, step);
        }
    }
}

char *JoinerEffect::getData(OutPort *, int readLength)
{
    // Only one outport
    char* outData = getOutPortData(outPorts.first(), readLength);

    char* inData1 = inPorts.at(0)->getData();
    char* inData2 = inPorts.at(1)->getData();

    if (effectMap->isPortConnected(inPorts.at(0)) && effectMap->isPortConnected(inPorts.at(1))){
        // Two valid inputs //WARNING maybe change to a better check system
        JoinerEffect::applyEffect(inData1, inData2, outData, readLength);
    } else if (inData1 != nullptr){
        memcpy(outData, inData1, readLength);
    } else if (inData2 != nullptr){
        memcpy(outData, inData2, readLength);
    }

    return outData;
}
