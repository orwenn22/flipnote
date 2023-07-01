#ifndef EDITORBRUSHBUTTON_H
#define EDITORBRUSHBUTTON_H

#include "EditorButton.h"

class FlipnoteEditor;
struct SDL_Texture;

class EditorBrushButton : public EditorButton {
    public:
    EditorBrushButton(
        WidgetContainer* container, FlipnoteEditor* editor,
        int x = 0, int y = 0,
        WidgetAllign allignment = WidgetAllign::WidgetAllign_None
    );
    virtual ~EditorBrushButton();

    void PreUpdate() override;
    void Render() override;

    private:
    //When the brush's settings are modified, we need to update the preview of the button too.
    void UpdateTexture();

    SDL_Texture* m_texture;
    int m_displayedbrushsize;
    int m_displayedbrushkind;
    bool m_displayedbrushinverted;
};

#endif
