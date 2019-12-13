#ifndef EFFECTUI_H
#define EFFECTUI_H

#include <QObject>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class Effect;
class GUI_effect;
class EffectMap;
class GUI_port;
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

    QList<GUI_effect*> m_effects;

    QList<QString> effectTypeList;
    QComboBox* effectsSelect;
    QPushButton* newEffectButton;

    int newEffectType = 0;

    GUI_port* getContainingPort(QPointF point);
    bool portLineDrag = false;
    QLineF* portLine;
    GUI_port* port_ptr;

    QTransform deviceTransform;
    bool dragging = false;
    QPointF dragStartPosition;
    QPointF dragRelativePosition;
    QGraphicsItem* draggedItem = nullptr;

signals:

public slots:
    // QGraphicsScene interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
#define mousemove
#ifdef mousemove
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
#endif
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // EFFECTUI_H
