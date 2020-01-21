#ifndef EFFECTUI_H
#define EFFECTUI_H

#include <QObject>
#include <QWidget>

QT_BEGIN_NAMESPACE
class Effect;
class EffectMap;
class Port;
class QGridLayout;
class QComboBox;
class QGraphicsScene;
class QPushButton;
QT_END_NAMESPACE

#define GRAPHICS_UI


#ifndef GRAPHICS_UI

class EffectsUI : public QWidget
{
    Q_OBJECT
public:
    explicit EffectsUI(QWidget *parent = nullptr);
    QGridLayout* mainLayout;

    //dump
    void setupEffectsSelect(QComboBox* effectsSelect);
    void createEffectUI(Effect* e);
    void deleteEffectUI(Effect* e);

    int getNewEffectType() const;

    QGraphicsScene *getScene() const;

private:
    EffectMap* effectMap;

    QGraphicsScene* scene;

    QList<QString> effectTypeList;
    QComboBox* effectsSelect;
    QPushButton* newEffectButton;

    int newEffectType = 0;



public slots:
};
#endif
#endif // EFFECTUI_H
