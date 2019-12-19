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
    explicit Effect(EffectMap *parent = nullptr);

    EffectMap* getEffectMap(){return effectMap;}

    virtual void applyEffect(char* in, char* out, int readLength);
    void applyEffect(char *data, int readLength);
    QString effectName = "default effect";

    //"Non-static protected memers can not be accessed via a pointer to the base class." wot>?
    QList<Parameter*>* getParamList() {return &m_parameters;}

    QList<Port*> getPorts();

    virtual char* getData();

    GUI_effect *getUi() const;
    void setUi(GUI_effect *value);

protected:
    GUI_effect* ui;

    QList<InPort*> m_inPortList;
    QList<OutPort*> m_outPortList;
    QList<Parameter*> m_parameters;

    void addParameter(Parameter* param, QString name); //TODO Add "connect" portion (lambda expr?)
    SliderParam* addSliderParameter(QString name, int min, int max, int val);

private:
    QGroupBox* frame = nullptr;
    bool bUIGenerated = false;

    EffectMap* effectMap = nullptr;

    QPushButton *deleteButton;

};


#endif // EFFECT_H
