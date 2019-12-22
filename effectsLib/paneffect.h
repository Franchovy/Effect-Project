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
    void applyEffect(char *in, char *out, int readLength);
private:
    int speed = 0;
    SliderParam* speedParam = nullptr;
    int stereocount = 0;
    int pancount = 1;
    double panVal;
    int panspeedtest = 200000;

    //working variable
    int x; // try uint8_t

};

#endif // PANEFFECT_H
