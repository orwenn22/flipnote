#ifndef LAYERBUTTON_H
#define LAYERBUTTON_H

#include "EditorButton.h"

class LayerButton : public EditorButton {
    public:

    LayerButton(
        FlipnoteEditor* editor, int layerindex,
        int x = 0, int y = 0, int w = 10, int h = 10,         //x and y are actually the offset
        WidgetAllign allignment = WidgetAllign::WidgetAllign_None
    );

    void Render() override;

    private:
    int m_layerindex;
};

#endif
