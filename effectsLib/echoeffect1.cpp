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

    effectBufferpt = 0;
    /*effectBuffer = QByteArray(len, 0);
    effectBuffer.fill(0);*/

    delayVal = 2;
    resizeBuffer(5000);
}
//In and Out data buffers
void EchoEffect1::applyEffect(char *in, char *out, int readLength){
    /*if (readLength == -1){
        //Calculate readLength searching for /0 char
        readLength = 0;
        while (in[readLength] != '\0'){
            readLength++;
        }
    }*/
    for (int i = 0; i < readLength; i += 2){
        if (format.sampleType() == QAudioFormat::SignedInt){
            int16_t x;
            memcpy(&x, &in[i], 2);
            int16_t b;
            memcpy(&b, &effectBuffer[effectBufferpt], 2);
            x += b;
            x = static_cast<int16_t>(x / static_cast<float>(delayVal));
            memcpy(&out[i], &x, 2);
            memcpy(&effectBuffer[effectBufferpt], &x, 2);
        }
        //effectBuffer[effectBufferpt-1] = (0x00ff & in[i]) + (0x00ff & static_cast<char>(effectBuffer[effectBufferpt] / static_cast<float>(delayVal)));
        //out[i] = effectBuffer[effectBufferpt];

        effectBufferpt += 1;
        if (effectBufferpt > len) {
            effectBufferpt = 2;
        }
    }
}

char *EchoEffect1::getData(OutPort*, int readLength)
{
    // Only one outport
    char* outData = getOutPortData(outPorts.first(), readLength);
    //TODO resize buffer here based on readLength;

    EchoEffect1::applyEffect(inPorts.first()->getData(), outPorts.first()->data, readLength);
    return outData;
}

void EchoEffect1::resizeBuffer(int newSize){
    newSize *= 8;
    len = newSize;

    //qDebug() << "Resizing buffer";
    effectBuffer = new int16_t[len];
    //effectBuffer.resize(newSize);
    //effectBuffer.fill(0);
}

