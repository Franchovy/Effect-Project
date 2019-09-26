#ifndef ECHOEFFECT1_H
#define ECHOEFFECT1_H

#include <effect.h>



class EchoEffect1 : public Effect
{
public:
    EchoEffect1();

    void applyEffect(char *in, char *out, int readLength);

private:
    QByteArray effectBuffer;
    int effectBufferpt;
    int len;
};

#endif // ECHOEFFECT1_H
