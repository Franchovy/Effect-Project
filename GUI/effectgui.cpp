#include "effectgui.h"

#include <QPainter>

EffectGUI::EffectGUI(QString name, QGraphicsItem* parent) : QGraphicsItem(parent)
{
    baseRect = QRectF(-200,-200,200,200);
    title = name;
}

QRectF EffectGUI::boundingRect() const
{
    return baseRect;
}

void EffectGUI::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QColor(0,0,0));


    //Set up title properly with boundingrect and font
    QFont font = painter->font();
    font.setPixelSize(20);
    painter->setFont(font);



    painter->drawRect(baseRect);
    //painter->drawRoundedRect(-10,-10,20,20,5,5);
    painter->drawText(QPoint(baseRect.topLeft().x()+10, baseRect.topLeft().y()+10), title);
}
