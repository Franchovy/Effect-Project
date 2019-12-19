#ifndef EFFECTUI_H
#define EFFECTUI_H

#include <QObject>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class Effect;
class EffectMap;
class GUI_effect;
class GUI_line;
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
    GUI_effect* addEffect(Effect* e);
    void deleteEffect(Effect* e);

    int getNewEffectType() const;

    GUI_port* getGUI_port(Port* p);
    GUI_effect* getGUI_effect(Effect* e);

    QGraphicsView* getView();
    void setView(QGraphicsView *value);

    // QGraphicsScene interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    EffectMap* effectMap;

    QList<GUI_effect*> m_effects;

    QList<QString> effectTypeList;
    QComboBox* effectsSelect;
    QPushButton* newEffectButton;

    int newEffectType = 0;

    GUI_port* getContainingPort(QPointF point);
    bool portLineDrag = false;
    GUI_line* portLine = nullptr;
    QPair<GUI_line*,GUI_line*> portLines;
    QPointF portDragPoint;
    GUI_port* port_ptr;

    QGraphicsView* view;

    QTransform deviceTransform;
    bool dragging = false;
    QGraphicsItem* draggedItem = nullptr;
    bool dragView = false;

    Effect* effectAt(QPointF point);

signals:
    void connectPortsSignal(Port* p1, Port* p2);

public slots:
    void effect_constructor(Effect* ptr);
    GUI_line* connectPorts(Port* p1, Port* p2);

};

#endif // EFFECTUI_H
