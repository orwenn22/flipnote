#ifndef POPUPMENUTOP_H
#define POPUPMENUTOP_H

#include "Widget.h"

//TODO : get rid of this and find a better way of solving whatever problem led to the creation of this atrocity.
class PopupMenuTop : public Widget {
    public:
    PopupMenuTop(WidgetContainer* container, int width);

    void Update();
    void Render();
};

#endif
