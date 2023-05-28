#ifndef EDITORBUTTON_H
#define EDITORBUTTON_H

#include <functional>

#include "../Generic/Widget.h"

class EditorButton; //forward declaration used by EditorButtonCallback
class FlipnoteEditor;
struct SDL_Texture;

using EditorButtonCallback = std::function<void(EditorButton*)>;



//This is a button contained in FlipnoteEditor.
class EditorButton : public Widget {
    public:

    EditorButton(
        WidgetContainer* container,
        FlipnoteEditor* editor, 
        int x = 0, int y = 0, int w = 10, int h = 10,         //x and y are actually the offset
        WidgetAllign allignment = WidgetAllign::WidgetAllign_None, 
        EditorButtonCallback callback = [](EditorButton*) -> void {}
    );
    virtual ~EditorButton();

    virtual void Update();
    virtual void Render();

    protected:
    //Executed when the button is clicked
    EditorButtonCallback m_callback;

    FlipnoteEditor* m_editor;
};

#endif