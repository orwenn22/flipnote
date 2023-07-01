#ifndef BRUSHSIZEBUTTON_H
#define BRUSHSIZEBUTTON_H

#include "../../../Reusable/gui/ClickableWidget.h"

class FlipnoteEditor;
struct SDL_Texture;

class BrushSizeButton : public ClickableWidget {
    public:
    BrushSizeButton(WidgetContainer* container, FlipnoteEditor* editor, int brushsize, int x, int y);
    ~BrushSizeButton();

    void Render() override;

    private:
    int m_brushsize;
    SDL_Texture* m_texture;
    FlipnoteEditor* m_editor;
};

#endif
