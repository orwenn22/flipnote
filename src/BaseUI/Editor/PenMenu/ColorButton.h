#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include "../../../Reusable/gui/ClickableWidget.h"

class FlipnoteEditor;

class ColorButton : public ClickableWidget {
    public:
    //index is the color to switch to when the button is clicked
    ColorButton(WidgetContainer* container, FlipnoteEditor* editor, int index, int x, int y);

    void Render() override;

    private:
    FlipnoteEditor* m_editor;
    int m_index;
};

#endif
