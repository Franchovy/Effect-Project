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
class QState;
class QStateMachine;
QT_END_NAMESPACE


class EffectsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit EffectsScene(QWidget *parent = nullptr);
    QGridLayout* mainLayout;

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

    QList<QGraphicsItem*>* selectedItems;

    QSet<Effect*>* m_effects;
    QMap<Effect*, QList<QPointF>>* m_effectPorts;
    QMap<GUI_effect*, Effect*>* m_GUIeffects;
    QMap<GUI_port*, Effect*>* m_GUIports;
    QMap<QPair<QPair<Effect*,int>,QPair<Effect*,int>>, GUI_line*>* m_connections; // QPair<Effect*,int> should be defined as a macro, or replaced with ID./

    QPointF newEffectPos(Effect*);
    QList<GUI_port*> getPorts(Effect*);
    Effect* getEffectAt(QPointF);
    QPointF getPortCenter(QPointF);
    GUI_port* getPortAt(QPointF);
    int getPortNumber(QPointF);
    GUI_line* getConnection(Effect*, int);
    void connectLine();

    void drag(QPointF p);
    double dragDistance = 0;

    enum mouseStates {neutral, dragging, linedrag, splitlines, selecting, deselecting};
    mouseStates mouseState;

    /* //TODO move to MainWindow
    QComboBox* effectsSelect;
    QPushButton* newEffectButton;
    */
    bool portLineDrag = false;
    GUI_line* portLine = nullptr;
    QPair<GUI_line*,GUI_line*> portLines;
    QPointF portDragPoint;
    GUI_port* port_ptr;

    QGraphicsView* view;

    void splitLineStart(QPointF p);
    void splitLineDrag(QPointF p);
    void connectSplitLines();
    void splitLineErase();

    QTransform transform;
    //bool dragging = false;
    //QGraphicsItem* draggedItem = nullptr;
    //bool dragView = false;

signals:
//  Signals relay to audio
    void newEffectSignal(int effectType);
    void deleteEffectSignal(Effect* e);
    void connectPortsSignal(QPair<Effect*, int>, QPair<Effect*, int>);
    void disconnectPortsSignal(QPair<Effect*, int>, QPair<Effect*, int>);

public slots:
    void addEffect(Effect*);
    void deleteEffect(Effect*);
    void disconnectPorts(QPair<Effect*, int>, QPair<Effect*, int>);
    //GUI_line* connectPorts(QPair<Effect*, int>, QPair<Effect*, int>);


    // QGraphicsScene interface
protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // EFFECTUI_H
