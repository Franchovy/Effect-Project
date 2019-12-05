#ifndef EFFECTUI_H
#define EFFECTUI_H

#include <QObject>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class Effect;
class EffectGUI;
class EffectMap;
class Port;
class QGridLayout;
class QComboBox;
class QGraphicsScene;
class QPushButton;
QT_END_NAMESPACE


class EffectsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit EffectsScene(QWidget *parent = nullptr);
    QGridLayout* mainLayout;

    //dump
    void setupEffectsSelect(QComboBox* effectsSelect);
    void createEffectUI(Effect* e);
    void deleteEffectUI(Effect* e);

    int getNewEffectType() const;

private:
    EffectMap* effectMap;

    QList<EffectGUI*> m_effects;

    QList<QString> effectTypeList;
    QComboBox* effectsSelect;
    QPushButton* newEffectButton;

    int newEffectType = 0;


signals:

public slots:

    // QGraphicsScene interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // EFFECTUI_H
