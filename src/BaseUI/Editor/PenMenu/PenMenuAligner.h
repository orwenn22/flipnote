#ifndef PENMENUALIGNER_H
#define PENMENUALIGNER_H

#include "../../Generic/PopupMenuItem.h"

class EditorPenButton;

//This object is used to place the popupmenu at the left of the pen button
class PenMenuAligner : public PopupMenuItem {
    public:
    PenMenuAligner(PopupMenu* popupmenu, EditorPenButton* penbutton);

    void Update();
    void Render();

    private:
    EditorPenButton* m_penbutton;
};

#endif