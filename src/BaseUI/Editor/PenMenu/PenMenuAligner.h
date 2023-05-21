#ifndef PENMENUALIGNER_H
#define PENMENUALIGNER_H

#include "../../Generic/Widget.h"

class EditorPenButton;

//This object is used to place the popupmenu at the left of the pen button
class PenMenuAligner : public Widget {
    public:
    PenMenuAligner(WidgetContainer* container, EditorPenButton* penbutton);

    void Update();
    void Render();

    private:
    EditorPenButton* m_penbutton;
};

#endif