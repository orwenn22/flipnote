#ifndef EDITORTIMELINEBUTTON_H
#define EDITORTIMELINEBUTTON_H

#include "EditorButton.h"


class EditorTimelineButton : public EditorButton {
    public:
    //EditorTimelineButton(WidgetContainer* container, FlipnoteEditor* editor);
    EditorTimelineButton(
        WidgetContainer* container, FlipnoteEditor* editor,
        int x = 0, int y = 0,
        WidgetAllign allignment = WidgetAllign::WidgetAllign_None
    );

    void Render() override;
};

#endif
