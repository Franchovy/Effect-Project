#ifndef EFFECTGUI_H
#define EFFECTGUI_H

//#include <QObject>
#include <QGraphicsItem>

class EffectGUI : public QGraphicsItem
{
    //Q_OBJECT
public:
    EffectGUI(QString name = "Default name", QGraphicsItem* parent = nullptr);

private:
    QRectF baseRect;
    QString title;


    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // EFFECTGUI_H
