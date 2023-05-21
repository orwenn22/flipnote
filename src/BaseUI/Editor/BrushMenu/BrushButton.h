#ifndef BRUSHBUTTON_H
#define BRUSHBUTTON_H

#include "../../Generic/Widget.h"

class FlipnoteEditor;

struct SDL_Texture;

class BrushButton : public Widget {
    public:
    BrushButton(WidgetContainer* container, FlipnoteEditor* editor, int brushindex, int x, int y);
    virtual ~BrushButton();

    void Update();
    void Render();

    private:

    void InitTexture();

    int m_displayedbrushinverted;
    int m_brushindex;
    FlipnoteEditor* m_editor;
    SDL_Texture* m_brushtexture;
};

#endif