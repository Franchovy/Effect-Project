#ifndef EFFECTMAP_H
#define EFFECTMAP_H

#include <QObject>

QT_BEGIN_NAMESPACE
class Effect;
class Port;
QT_END_NAMESPACE

class EffectMap : public QObject
{
    Q_OBJECT
public:
    explicit EffectMap(QObject *parent = nullptr);

    void addEffect(Effect* e);
    void addPort(Effect* e, Port*);

    void connectPorts(Port* p1, Port* p2);

    QList<Port*> getPorts(Effect* e);
    QList<Port*> getPortsOfType(int type);
    QList<Port*> getFreePorts();
    QList<Port*> getFreePortsOfType(int type);

private:
    QHash<Effect*, QList<Port*>>* m_effectMap;
    QHash<Port*, Port*>* m_connectionsMap;
signals:

public slots:
};

#endif // EFFECTMAP_H
