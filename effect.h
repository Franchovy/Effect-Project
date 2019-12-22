#ifndef EFFECT_H
#define EFFECT_H

#include <QObject>
#include <QWidget>

#include "Params/parameter.h"
#include "Params/sliderparam.h"

QT_BEGIN_NAMESPACE
class Audio;
class EffectMap;
class GUI_effect;
class InPort;
class OutPort;
class Port;
class QComboBox;
class QGroupBox;
class QPushButton;
QT_END_NAMESPACE

//TODO DEFINE EFFECTS HERE

/*
// Link to classes in creation switch-statement.
enum EffectTypes
{
    echo1 = 0,
    fuzz,
    pan,
    end
};
*/

class Effect : public QObject
{
    Q_OBJECT
public:
    explicit Effect(Audio *parent = nullptr);

    virtual void applyEffect(char* in, char* out, int readLength);
    void applyEffect(char *data, int readLength);
    QString effectName = "default effect";

    QList<Port*> getPorts();

    //"Non-static protected memers can not be accessed via a pointer to the base class." wot>?
    QList<Parameter*>* getParamList() {return &m_parameters;}

    virtual char* getData();
protected:
    QList<Port*> ports;
    QList<QPointF> portLocs;
    QList<Parameter*> m_parameters;
};


#endif // EFFECT_H
