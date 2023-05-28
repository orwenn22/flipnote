#ifndef EDITORPENBUTTON_H
#define EDITORPENBUTTON_H

#include "EditorButton.h"


class EditorPenButton : public EditorButton {
    public:
    EditorPenButton(WidgetContainer* container, FlipnoteEditor* editor);

    void Render() override;
};

#endif