#ifndef EFFECTMAP_H
#define EFFECTMAP_H

#include <QObject>

QT_BEGIN_NAMESPACE
class Audio;
class Effect;
class InputEffect;
class OutputEffect;
class Port;
QT_END_NAMESPACE

class EffectMap : public QObject
{
    Q_OBJECT
public:
    explicit EffectMap(Audio *parent = nullptr);

    void addEffect(Effect* e);
    void addPort(Effect* e, Port*);
    Effect* createEffect(int effectType);

    void connectPorts(Port* p1, Port* p2);

    QList<Port*> getPorts(Effect* e);
    QList<Port*> getPorts();
    QList<Port*> getPortsOfType(int type);
    QList<Port*> getFreePorts();
    QList<Port*> getFreePortsOfType(int type);

    void createDefaultInputOutputEffects(InputEffect* in_e, OutputEffect* out_e);

private:
    //default input and output effects
    InputEffect* inputEffect;
    OutputEffect* outputEffect;

    Audio* parentAudio;

    QHash<Effect*, QList<Port*>>* m_effectMap;
    QHash<Port*, Port*>* m_connectionsMap;
signals:
    void refreshPortLists();
public slots:
    void updatePortConnectionSelect(int type);
};

#endif // EFFECTMAP_H
