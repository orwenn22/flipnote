#ifndef EDITORPLAYBUTTON_H
#define EDITORPLAYBUTTON_H

#include "EditorButton.h"

class EditorPlayButton : public EditorButton {
    public:
    EditorPlayButton(
        WidgetContainer* container, FlipnoteEditor* editor,
        int x = 0, int y = 0,
        WidgetAllign allignment = WidgetAllign::WidgetAllign_None
    );

    void Render() override;
};

#endif
