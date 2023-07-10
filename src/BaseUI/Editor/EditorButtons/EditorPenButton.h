#ifndef EDITORPENBUTTON_H
#define EDITORPENBUTTON_H

#include "EditorButton.h"


class EditorPenButton : public EditorButton {
    public:
    EditorPenButton(FlipnoteEditor* editor);

    void Render() override;
};

#endif
