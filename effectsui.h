#ifndef EFFECTUI_H
#define EFFECTUI_H

#include <QObject>
#include <QWidget>

QT_BEGIN_NAMESPACE
class Effect;
class EffectMap;
class QGridLayout;
class QComboBox;
class QPushButton;
QT_END_NAMESPACE


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

private:
    EffectMap* effectMap;

    QList<QString> effectTypeList;
    QComboBox* effectsSelect;
    QPushButton* newEffectButton;

    int newEffectType = 0;


signals:

public slots:
};

#endif // EFFECTUI_H
