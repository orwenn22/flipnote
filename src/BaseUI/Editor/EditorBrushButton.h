#ifndef EDITORBRUSHBUTTON_H
#define EDITORBRUSHBUTTON_H

#include "../Generic/PopupMenuItem.h"

class FlipnoteEditor;
struct SDL_Texture;

class EditorBrushButton  {
    public:
    EditorBrushButton(FlipnoteEditor* editor);
    ~EditorBrushButton();

    void Update();
    void Render();

    int GetX(); //Necessary for BrushMenuAligner (not presentyet)

    private:
    void UpdatePos();   //Put the button at the right edge of the window

    //When the brush's settings are modified, we need to update the preview of the button too.
    void UpdateTexture();

    FlipnoteEditor* m_editor;

    int m_x;
    int m_y;
    int m_w;
    int m_h;

    SDL_Texture* m_texture;
    int m_displayedbrushsize;
    int m_displayedbrushkind;
    bool m_displayedbrushinverted;
};

#endif