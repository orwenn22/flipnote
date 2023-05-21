#ifndef POPUPMENUTOP_H
#define POPUPMENUTOP_H

#include "Widget.h"

class PopupMenuTop : public Widget {
    public:
    PopupMenuTop(WidgetContainer* container, int width);

    void Update();
    void Render();
};

#endif