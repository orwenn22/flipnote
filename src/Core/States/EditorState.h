#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include "../../Reusable/State/State.h"

class Background;
class Flipnote;
class FlipnoteEditor;
class TopBar;

class EditorState : public State {
    public:
    EditorState(Flipnote* flipnote = nullptr);
    ~EditorState();

    void Update();
    void Render();

    private:
    Background* m_background;
    TopBar* m_topbar;

    Flipnote* m_flipnote;       //Note : the EditorState deallocate the flipnote 
    FlipnoteEditor* m_editor;
};

#endif
