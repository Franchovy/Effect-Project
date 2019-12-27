#include "gui_item.h"

GUI_item::GUI_item(QGraphicsItem* parent) : QGraphicsItem(parent)
{

}

void GUI_item::select()
{
    selected = true;
    update();
}

void GUI_item::deselect()
{
    selected = false;
    update();
}
