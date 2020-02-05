#ifndef INPUTEFFECT_H
#define INPUTEFFECT_H

#include <QObject>
#include "effect.h"

QT_BEGIN_NAMESPACE
class OutPort;
QT_END_NAMESPACE


class InputEffect : public Effect
{
    Q_OBJECT
public:
    InputEffect(Audio* parent = nullptr);


    void giveData(char* indata, int readLength);
    char* giveData(float *indata, int readLength);
    char *getData(OutPort*, int) override;

private:
    OutPort* inputDevicePort;
    void applyEffect(char *in, char *out, int readLength) override;

    bool hasData = false;
    char* data = nullptr;
    int dataLength = 0;

    //temp test stuff
    int freqLimit = 400;
    int freq = freqLimit;
};

#endif // INPUTEFFECT_H
