#ifndef BRUSHBUTTON_H
#define BRUSHBUTTON_H

#include "../../../Reusable/gui/ClickableWidget.h"

class FlipnoteEditor;

struct SDL_Texture;

class BrushButton : public ClickableWidget {
    public:
    BrushButton(WidgetContainer* container, FlipnoteEditor* editor, int brushindex, int x, int y);
    virtual ~BrushButton();

    void PreUpdate() override;
    void Render() override;

    private:

    void InitTexture();

    int m_displayedbrushinverted;
    int m_brushindex;
    FlipnoteEditor* m_editor;
    SDL_Texture* m_brushtexture;
};

#endif
