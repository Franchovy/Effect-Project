#ifndef EFFECTMAP_H
#define EFFECTMAP_H

#include <QObject>

QT_BEGIN_NAMESPACE
class Audio;
class Effect;
class EffectsScene;
class InputEffect;
class OutputEffect;
class Port;
QT_END_NAMESPACE

class EffectMap : public QObject
{
    Q_OBJECT
public:
    explicit EffectMap(Audio *parent = nullptr);

    Effect* getEffectFromPort(Port* port);
    EffectsScene* getEffectsScene();

    char* getData(Effect* e);

    //void createDefaultInputOutputEffects(InputEffect* in_e, OutputEffect* out_e);

    int readLength = 0;

private:
    QMap<Effect*, Port*>* m_effectMap;
    QHash<Port*, Port*>* m_connectionsMap;

public Q_SLOTS:
    void addEffect(Effect* e);
    void deleteEffect(Effect* e);
    void connectPorts(Port* p1, Port* p2);
    void disconnectPorts(Port* p1, Port* p2);
};

#endif // EFFECTMAP_H
