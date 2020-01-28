#ifndef EFFECTUI_H
#define EFFECTUI_H

#include <QObject>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class Effect;
class EffectMap;
class GUI_effect;
class GUI_line;
class GUI_item;
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

    int getDragState() {return dragState;}

    QString compileSaveEffect();
    bool loadEffect(QPair<QList<QPair<int,int>>,QList<QPair<QPair<int,int>,QPair<int,int>>>>);

    // QGraphicsScene interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    EffectMap* effectMap;

    QSet<Effect*>* m_effects;
    QMap<Effect*, QList<GUI_port*>>* m_effectPorts;
    QMap<GUI_effect*, Effect*>* m_GUIeffects;
    QMap<GUI_port*, Effect*>* m_GUIports;
    QMap<QPair<QPair<Effect*,int>,QPair<Effect*,int>>, GUI_line*>* m_connections; // QPair<Effect*,int> should be defined as a macro, or replaced with ID./

    Effect* newEffect;
    int posCount = 0;
    GUI_line* getConnection(Effect*, int);
    void connectLine(GUI_port*, QPointF);

    QList<GUI_port*> getPortsOfType(Effect*, int);

    void dragItem(GUI_item*, QPointF);
    double dragDistance = 0;

    enum mouseStates {neutral, drag, line, view};
    mouseStates dragState;
    bool shiftState = false;

    bool ctrlState = false;

    QList<GUI_item*> selectedItems;
    void clearSelectedItems();
    GUI_item* clicked_item;
    GUI_item* hover_item;
    QList<GUI_item*> getItemsAt(QPointF);
    int scrollRef = 0;

    bool portLineDrag = false;
    GUI_line* portLine = nullptr;
//    QPair<GUI_line*,GUI_line*> portLines;
    QPointF portDragPoint;
    GUI_port* port_ptr;

    QGraphicsView* graphicsView;

    void select(GUI_item*);
    void deselect(GUI_item*, bool clear = false);

    QTransform transform;

    void dragLine(GUI_port *port, QPointF pos);
    GUI_item *getScrollSelected(QList<GUI_item *> list);
Q_SIGNALS:
//  Signals relay to audio
    void newEffectSignal(int effectType);
    void deleteEffectSignal(Effect* e);
    void connectPortsSignal(QPair<Effect*, int>, QPair<Effect*, int>);
    void disconnectPortsSignal(QPair<Effect*, int>, QPair<Effect*, int>);

    void runAudioSignal();
    void stopAudioSignal();


public Q_SLOTS:
    void addEffect(Effect*);
    void deleteEffect(Effect*);
    void disconnectPorts(QPair<Effect*, int>, QPair<Effect*, int>);
    void connectPorts(QPair<Effect*, int>, QPair<Effect*, int>);


    // QGraphicsScene interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
};

#endif // EFFECTUI_H
