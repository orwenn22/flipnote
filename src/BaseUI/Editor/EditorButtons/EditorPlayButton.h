#ifndef EDITORPLAYBUTTON_H
#define EDITORPLAYBUTTON_H

#include "EditorButton.h"

class EditorPlayButton : public EditorButton {
    public:
    EditorPlayButton(WidgetContainer* container, FlipnoteEditor* editor);

    void Render() override;
};

#endif
