#include "paneffect.h"

#include "ports/inport.h"
#include "ports/outport.h"

#include <QDebug>
#include <QtMath>

PanEffect::PanEffect(Audio* parent) : Effect(parent)
  , inPort(new InPort("Pan In", this))
  , outPort(new OutPort("Pan Out", this))
{
    effectName = "Pan Effect";
    type = 3;

    addPort(inPort);
    addPort(outPort);

    /*
    speedParam = addSliderParameter("Speed", 1, 1000, 10);
    QObject::connect(speedParam, &SliderParam::valueChanged, [this](int value){
        panspeedtest = value * 100;
        if (pancount <= panspeedtest) pancount = 1;


    });*/
    panVal = static_cast<double>(pancount) / (panspeedtest / 2);

    //step = format.sampleSize()/8 * format.channelCount();
}

void PanEffect::applyEffect(char *in, char *out, int readLength)
{
    for (int i = 0; i < readLength; i += step){
        if (stereocount <= 1){
            //Left
            if (format.sampleType() == QAudioFormat::SignedInt){
                int16_t x;
                memcpy(&x, &in[i], step);
                x = static_cast<int16_t>(x * abs(1 - panVal));
                memcpy(&out[i], &x, step);
            } else if (format.sampleType() == QAudioFormat::Float){
                float x;
                memcpy(&x, &in[i], step);
                x = static_cast<float>(x * abs(1 - panVal));
                memcpy(&out[i], &x, step);
            }
        } else {
            //Right
            if (format.sampleType() == QAudioFormat::SignedInt){
                int16_t x;
                memcpy(&x, &in[i], step);
                if (panVal > 1) x = static_cast<int16_t>(x * abs(2 - panVal));
                else x = static_cast<int16_t>(x * abs(-panVal));
                memcpy(&out[i], &x, step);
            } else if (format.sampleType() == QAudioFormat::Float){
                float x;
                memcpy(&x, &in[i], step);
                if (panVal > 1) x = static_cast<float>(x * abs(2 - panVal));
                else x = static_cast<float>(x * abs(-panVal));
                memcpy(&out[i], &x, step);
            }
        }

        stereocount += 2;
        if (stereocount == 4) stereocount = 0;
        pancount ++;
        panVal = static_cast<double>(pancount) / (panspeedtest / 2);
        if (pancount == panspeedtest-1) {
            pancount = 1;
        }
    }
}

char *PanEffect::getData(OutPort*, int readLength)
{
    // Only one outport
    char* outData = getOutPortData(outPorts.first(), readLength);
    PanEffect::applyEffect(inPorts.first()->getData(), outPorts.first()->data, readLength);
    return outData;
}
