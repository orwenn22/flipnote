#ifndef EDITORBUTTON_H
#define EDITORBUTTON_H

#include <functional>

#include "../../../Reusable/gui/ClickableWidget.h"

class EditorButton; //forward declaration used by EditorButtonCallback
class FlipnoteEditor;
struct SDL_Texture;


//This is a button contained in FlipnoteEditor.
class EditorButton : public ClickableWidget {
    public:

    EditorButton(
        WidgetContainer* container,
        FlipnoteEditor* editor, 
        int x = 0, int y = 0, int w = 10, int h = 10,         //x and y are actually the offset
        WidgetAllign allignment = WidgetAllign::WidgetAllign_None, 
        ClickabbleWidgetCallback callback = []() -> void {}
    );
    virtual ~EditorButton();

    protected:
    FlipnoteEditor* m_editor;
};

#endif
