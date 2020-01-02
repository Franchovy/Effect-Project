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


    void giveData(char* data, int readLength);
    char *getData(OutPort*, int) override;


private:
    OutPort* inputDevicePort;

    void applyEffect(char *in, char *out, int readLength) override;

    bool hasData = false;
    char* data = nullptr;
};

#endif // INPUTEFFECT_H
