#ifndef EDITORPENBUTTON_H
#define EDITORPENBUTTON_H

class FlipnoteEditor;

class EditorPenButton {
    public:
    EditorPenButton(FlipnoteEditor* editor);

    void Update();
    void Render();

    int GetX(); //Necessary for PenMenuAligner

    private:
    void UpdatePos();   //Put the button at the right edge of the window

    FlipnoteEditor* m_editor;

    int m_x;
    int m_y;
    int m_w;
    int m_h;
};

#endif