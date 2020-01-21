#ifndef PANEFFECT_H
#define PANEFFECT_H

#include <QObject>
#include <effect.h>

class PanEffect : public Effect
{
    Q_OBJECT
public:
    explicit PanEffect(Audio* parent = nullptr);

    // Effect interface
public:
    void applyEffect(char *in, char *out, int readLength) override;
    char *getData(OutPort*, int) override;

private:
    InPort *inPort;
    OutPort *outPort;

    int step;

    int speed = 0;
    SliderParam* speedParam = nullptr;
    int stereocount = 0;
    int pancount = 1;
    double panVal;
    int panspeedtest = 200000;

    //working variable
    int16_t x;
    int16_t x2;
};

#endif // PANEFFECT_H
