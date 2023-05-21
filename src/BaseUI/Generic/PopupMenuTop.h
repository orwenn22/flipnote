#ifndef POPUPMENUTOP_H
#define POPUPMENUTOP_H

#include "PopupMenuItem.h"

class PopupMenuTop : public PopupMenuItem {
    public:
    PopupMenuTop(PopupMenu* popupmenu, int width);

    void Update();
    void Render();
};

#endif