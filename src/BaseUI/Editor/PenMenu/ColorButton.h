#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include "../../Generic/PopupMenuItem.h"

class FlipnoteEditor;

class ColorButton : public PopupMenuItem {
    public:
    //index is the color to switch to when the button is clicked
    ColorButton(PopupMenu* popupmenu, FlipnoteEditor* editor, int index, int x, int y);

    void Update();
    void Render();

    private:
    FlipnoteEditor* m_editor;
    int m_index;
};

#endif