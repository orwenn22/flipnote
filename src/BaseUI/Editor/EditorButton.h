#ifndef EDITORBUTTON_H
#define EDITORBUTTON_H

#include <functional>

class EditorButton; //forward declaration used by EditorButtonCallback
class FlipnoteEditor;
struct SDL_Texture;

using EditorButtonCallback = std::function<void(EditorButton*)>;

enum ButtonAllign {
    ButtonAllign_None = 0b00,
    ButtonAllign_Left = 0b01,
    ButtonAllign_Bottom = 0b10,
    ButtonAllign_BottomLeft = 0b11
};

//This is a button contained in FlipnoteEditor.
//TODO : make it a widget ?
class EditorButton {
    public:

    EditorButton(
        FlipnoteEditor* editor, 
        int x = 0, int y = 0, int w = 10, int h = 10,         //x and y are actually the offset
        ButtonAllign allignment = ButtonAllign::ButtonAllign_None, 
        EditorButtonCallback callback = [](EditorButton*) -> void {}
    );
    virtual ~EditorButton();

    virtual void Update();
    virtual void Render();

    int GetX();
    int GetY();

    protected:
    //Put the button at the right edge of the window
    void UpdatePos();

    //Executed when the button is clicked
    EditorButtonCallback m_callback;

    FlipnoteEditor* m_editor;

    //absolute position
    int m_x;
    int m_y;
    int m_w;
    int m_h;

    //Used for allignment (in UpdatePos)
    int m_xoffset;
    int m_yoffset;
    ButtonAllign m_allignment;
};

#endif