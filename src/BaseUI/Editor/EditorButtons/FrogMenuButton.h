#ifndef FROGMENUBUTTON_H
#define FROGMENUBUTTON_H

#include "EditorButton.h"

class FrogMenuButton  : public EditorButton {
    public:
    FrogMenuButton(
        WidgetContainer* container, FlipnoteEditor* editor,
        int x = 0, int y = 0,
        WidgetAllign allignment = WidgetAllign::WidgetAllign_None
    );

    void Render() override;
};

#endif
