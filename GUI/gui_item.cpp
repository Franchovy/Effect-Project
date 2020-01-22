#include "gui_item.h"

GUI_item::GUI_item(QGraphicsItem* parent) : QGraphicsItem(parent)
{
    setAcceptHoverEvents(true);
}

void GUI_item::select()
{
    selected = true;
    update();
}

void GUI_item::deselect()
{

    if (this->selected){
        selected = false;
        update();
    }
}
