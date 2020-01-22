#ifndef EFFECT_H
#define EFFECT_H

#include <QObject>
#include <QWidget>

#include <QAudioFormat>

#include "ports/port.h"
#include "ports/inport.h"
#include "ports/outport.h"
#include "Params/parameter.h"
#include "Params/sliderparam.h"

QT_BEGIN_NAMESPACE
class Audio;
class EffectMap;
class GUI_effect;
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

    int type;

    EffectMap* effectMap;

    QList<QPointF> getPortLocs();
    QList<Port*> getPorts();

    //"Non-static protected memers can not be accessed via a pointer to the base class." wot>?
    QList<Parameter*>* getParamList() {return &m_parameters;}

    virtual char* getData(OutPort*, int readLength);

protected:
    void addPort(Port*, QPointF);
    void addParam(Parameter*);

    QList<InPort*> inPorts;
    QList<OutPort*> outPorts;

    QAudioFormat format;

    float getFloat(char* c);
    int16_t getInt(char *c);

    int step;

    char* getOutPortData(Port *port, int readLength);
private:
    QMap<Port*, QPointF>* m_ports;
    QList<Parameter*> m_parameters;

};


#endif // EFFECT_H
