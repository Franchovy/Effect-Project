#include "paneffect.h"
#include <QDebug>
#include <QtMath>

PanEffect::PanEffect()
{
    effectName = "Pan Effect";

    speedParam = addSliderParameter("Speed", 1, 10, 3);
    QObject::connect(speedParam, &SliderParam::valueChanged, [this](int value){
        speed = value;
    });
}

void PanEffect::applyEffect(char *in, char *out, int readLength)
{
    for (int i = 0; i < readLength; i += 2){
        //Assuming stereo is 24 bit, aka 3 bytes per side
        if (stereocount <= 1){
//#define NO_EFFECT_TEST
#ifdef NO_EFFECT_TEST
            out[i] = in[i];
            out[i+1] = in[i+1];
#else
            //Left
            x = in[i+1];
            x = x << 8;
            x = x | (0x00ff & in[i]);
            //x *= 0.1;
            x = static_cast<int>(x * abs(1 - panVal));
            //qDebug() << "Left: " << (sin(static_cast<double>(pancount)/panspeedtest)+1)/2;
            out[i] = x & 0x00ff;
            out[i+1] = (x & 0xFF00) >> 8;
#endif
        } else {
#ifdef NO_EFFECT_TEST
            out[i] = in[i];
            out[i+1] = in[i+1];
#else
            //Right
            x = in[i+1];
            x = x << 8;
            x = x | (0x00ff & in[i]);
            if (panVal > 1) x = static_cast<int>(x * abs(2 - panVal));
            else x = static_cast<int>(x * abs(-panVal));
//            //qDebug() << "Right: " << (cos(static_cast<double>(pancount)/panspeedtest)+1)/2;

            out[i] = x & 0x00ff;
            out[i+1] = (x & 0xFF00) >> 8;
#endif
        }
        pancount ++;
        stereocount += 2;
        if (stereocount == 4) stereocount = 0;

        panVal = static_cast<double>(pancount) / (panspeedtest / 2);
        //qDebug() << panVal;
        if (pancount == panspeedtest-1) {
            //qDebug() << "Loop";
            pancount = 1;
        }

    }
}