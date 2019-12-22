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

    QMap<Effect*, QPointF>* m_effects;
    QMap<Effect*, QPointF[]>* m_effectPorts;
    QMap<QPair<QPair<Effect*,int>,QPair<Effect*,int>>, GUI_line*>* m_connections; // QPair<Effect*,int> should be defined as a macro, or replaced with ID./

    QPointF newEffectPos(Effect*);
    QList<GUI_port*> getPorts(Effect*);
    Effect* getEffectAt(QPointF);
    QPointF getPortCenter(QPointF);
    void connectLine();

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

    void splitLineCreate(QPointF p);
    void splitLineDrag(QPointF p);
    void splitLineErase();

    QTransform deviceTransform;
    bool dragging = false;
    QGraphicsItem* draggedItem = nullptr;
    bool dragView = false;

signals:
    void connectPortsSignal(QPair<Effect*, int>, QPair<Effect*, int>);

public slots:
    void addEffect(Effect*);

    GUI_line* connectPorts(QPair<Effect*, int>, QPair<Effect*, int>);

};

#endif // EFFECTUI_H
