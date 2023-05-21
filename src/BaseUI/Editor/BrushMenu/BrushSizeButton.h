#ifndef BRUSHSIZEBUTTON_H
#define BRUSHSIZEBUTTON_H

#include "../../Generic/PopupMenuItem.h"

class FlipnoteEditor;
struct SDL_Texture;

class BrushSizeButton : public PopupMenuItem {
    public:
    BrushSizeButton(PopupMenu* popupmenu, FlipnoteEditor* editor, int brushsize, int x, int y);
    ~BrushSizeButton();

    void Update();
    void Render();

    private:
    int m_brushsize;
    SDL_Texture* m_texture;
    FlipnoteEditor* m_editor;
};

#endif