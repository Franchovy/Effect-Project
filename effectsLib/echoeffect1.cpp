#include "echoeffect1.h"

#include <QtDebug>
#include "ports/inport.h"
#include "ports/outport.h"
#include "Params/sliderparam.h"


EchoEffect1::EchoEffect1(Audio* parent) : Effect(parent)
{
    effectName = "Echo Effect 1";

    type = 2;

    InPort* inPort = new InPort("Echo In", this);
    OutPort* outPort = new OutPort("Echo Out", this);

    addPort(inPort, QPointF(150,100));
    addPort(outPort, QPointF(50, 100));

    /*
    lenParam = addSliderParameter("Buffer Length", 800, 80000, len = 8000);
    QObject::connect(lenParam, &SliderParam::valueChanged, [this](int value){
        resizeBuffer(value);
    });

    delayParam = addSliderParameter("Delay speed", 1, 100, delayVal = 10);
    QObject::connect(delayParam, &SliderParam::valueChanged, [this](int newVal){
        delayVal = newVal;
    });
    */


    effectBuffer = QByteArray(len, 0);
    effectBufferpt = 1;
    effectBuffer.fill(0);

    delayVal = 2;
    resizeBuffer(5000);
}
//In and Out data buffers
void EchoEffect1::applyEffect(char *in, char *out, int readLength){
    if (readLength == -1){
        //Calculate readLength searching for /0 char
        readLength = 0;
        while (in[readLength] != '\0'){
            readLength++;
        }
    }
    for (int i = 0; i < readLength; i += 1){

        effectBuffer[effectBufferpt-1] = (0x00ff & in[i]) + (0x00ff & static_cast<char>(effectBuffer[effectBufferpt] / static_cast<float>(delayVal)));
        out[i] = effectBuffer[effectBufferpt];

        effectBufferpt += 1;
        if (effectBufferpt > len) {
            effectBufferpt = 2;
        }
    }
}

char *EchoEffect1::getData(OutPort*, int readLength)
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
    EchoEffect1::applyEffect(inPorts.first()->getData(), outPorts.first()->data, readLength);
    return outData;
}

void EchoEffect1::resizeBuffer(int newSize){
    newSize *= 8;
    len = newSize;

    //qDebug() << "Resizing buffer";
    effectBuffer.resize(newSize);
    effectBuffer.fill(0);
}

