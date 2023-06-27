#ifndef EDITORTIMELINEBUTTON_H
#define EDITORTIMELINEBUTTON_H

#include "EditorButton.h"


class EditorTimelineButton : public EditorButton {
    public:
    EditorTimelineButton(WidgetContainer* container, FlipnoteEditor* editor);

    void Render() override;
};

#endif
