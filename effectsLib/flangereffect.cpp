#include "flangereffect.h"

FlangerEffect::FlangerEffect(QObject* parent)
{
    strengthParam = addSliderParameter("Strength", 0, 10, strength = 5);
    connect(strengthParam, &SliderParam::valueChanged, [this](int value){
        strength = value;
    });
    velocityParam = addSliderParameter("Velocity", 1, 10, velocity = 1);
    connect(velocityParam, &SliderParam::valueChanged, [this](int value){
        velocity = value;
    });
    lengthParam = addSliderParameter("Length", 10000, 1000000, length = 100000);
    connect(lengthParam, &SliderParam::valueChanged, [this](int value){
        resizeBuffer(value);
    });


    flangerBuffer = QByteArray(length, 0);
    inBufferPt = 0;
    outBufferPt = 0;
    fwd = true;
    flangerBuffer.fill(0);

    resizeBuffer(length);
}

void FlangerEffect::applyEffect(char *in, char *out, int readLength)
{
    for (int i = 0; i < readLength; i++){
        flangerBuffer[inBufferPt] = in[i];
        out[i] = in[i] + flangerBuffer[outBufferPt];

        if (fwd){
            outBufferPt += 2*velocity;
            //PROBLEM: Skipping bytes must not be the right way to go. There must be a way
            //to "accelerate" reading through some sort of function combining bytes together.
            //Flanger will be put on hold until I understand the data stream better.
            //For example, I don't even understand the stereo format. Thus -Pan Effect- should come first.
        }
    }
}

void FlangerEffect::resizeBuffer(int newSize){
    newSize *= 8;
    length = newSize;

    flangerBuffer.resize(newSize);
    flangerBuffer.fill(0);
}
