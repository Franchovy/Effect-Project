#include "effect.h"

Effect::Effect(QObject* parent) : QObject(parent)
{

}

void Effect::applyEffect(char *in, char *out, int readLength)
{
    //default stream. Like no effect
    for (int i = 0; i < readLength; i++){
        out[i] = in[i];
    }
}
