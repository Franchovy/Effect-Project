#ifndef GUI_ITEM_H
#define GUI_ITEM_H

#include <QGraphicsItem>

class GUI_item : public QGraphicsItem
{
public:
    GUI_item(QGraphicsItem* parent = nullptr);

    void select();
    void deselect();

protected:
    bool selected = true;
};

#endif // GUI_ITEM_H
