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
    InputEffect(EffectMap* parent = nullptr);


    void giveData(char* data);

private:
    OutPort* inputDevicePort;

    void applyEffect(char *in, char *out, int readLength) override;
    char *getData() override;

    bool hasData = false;
    char* data = nullptr;
};

#endif // INPUTEFFECT_H
