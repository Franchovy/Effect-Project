#ifndef ECHOEFFECT1_H
#define ECHOEFFECT1_H

#include <effect.h>

QT_BEGIN_NAMESPACE
class SliderParam;
class InPort;
class OutPort;
QT_END_NAMESPACE

class EchoEffect1 : public Effect
{
    Q_OBJECT
public:
    explicit EchoEffect1(Audio* parent = nullptr);

    void applyEffect(char *in, char *out, int readLength) override;

    char * getData(OutPort*, int) override;

private:
    SliderParam* lenParam = nullptr;
    int len;

    SliderParam* delayParam = nullptr;
    int delayVal;
    double delayOpVal;

    char* effectBuffer;
    int effectBufferpt;

    void resizeBuffer(int newSize);
};

#endif // ECHOEFFECT1_H
