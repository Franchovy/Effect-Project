#include "waveeffect.h"

#include <QDebug>

WaveEffect::WaveEffect(Audio* parent) : Effect(parent)
{
    effectName = "Wave Effect";
    type = 4;

    InPort* inPort = new InPort("Wave In", this);
    OutPort* outPort = new OutPort("Wave Out", this);

    addPort(inPort, QPointF(150,100));
    addPort(outPort, QPointF(50, 100));

    vol = 0;
}

void WaveEffect::applyEffect(char *in, char *out, int readLength)
{
    for (int i = 0; i < readLength; i += 2){
        if (format.sampleType() == QAudioFormat::Float){
            float x = getFloat(&in[i]);
            x = static_cast<float>(x*vol);
            memcpy(&out[i], &x, step);
        } else if (format.sampleType() == QAudioFormat::SignedInt){
            int16_t x;//= getInt(&in[i]);
            memcpy(&x, &in[i], 2);
            x = static_cast<int16_t>(x*vol);
            memcpy(&out[i], &x, 2);
        }
    }

    vol += dir * dvol;
    if (vol >= 1 || vol <= 0){
        dir *= -1;
    }
}

char *WaveEffect::getData(OutPort *, int readLength)
{
    // Only one outport
    char* outData = getOutPortData(outPorts.first(), readLength);
    WaveEffect::applyEffect(inPorts.first()->getData(), outPorts.first()->data, readLength);
    return outData;
}
