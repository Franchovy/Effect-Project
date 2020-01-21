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

    addPort(inPort, QPointF(150,100));
    addPort(outPort, QPointF(50, 100));

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
    for (int i = 0; i < readLength; i += 2){
        //for (int c = 0; c < format.channelCount(); c++){
            /*if (format.sampleType() == QAudioFormat::Float){
                if (c % 2 == 0){
                    // First phase for even number channel

                } else if (c % 2 == 1){
                    // Second phase

                }
            }*/


            if (stereocount <= 1){
                //Left
                memcpy(&x, &in[i], 2);
                x = static_cast<int16_t>(x * abs(1 - panVal));
                memcpy(&out[i], &x, 2);//format.sampleSize()/8);

            } else {
                //Right
                memcpy(&x, &in[i], 2);

                if (panVal > 1) x = static_cast<int16_t>(x * abs(2 - panVal));
                else x = static_cast<int16_t>(x * abs(-panVal));

                memcpy(&out[i], &x, 2);
            }

            stereocount += 2;
            if (stereocount == 4) stereocount = 0;

            pancount ++;

            panVal = static_cast<double>(pancount) / (panspeedtest / 2);
            //qDebug() << panVal;
            if (pancount == panspeedtest-1) {
                //qDebug() << "Loop";
                pancount = 1;
            }
        }
    //}
}

char *PanEffect::getData(OutPort*, int readLength)
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
    PanEffect::applyEffect(inPorts.first()->getData(), outPorts.first()->data, readLength);
    return outData;
}
