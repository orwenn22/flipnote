#ifndef MENUALIGNER_H
#define MENUALIGNER_H

#include "../../../Reusable/gui/Widget.h"

class EditorButton;

//This class will change the position of the popup menu (container) to put it next to the button it comes form.
//TODO : add configurable offset
class MenuAligner : public Widget {
    public:
    MenuAligner(WidgetContainer* container, EditorButton* penbutton);

    void Update();
    void Render();

    private:
    EditorButton* m_editorbutton;
};

#endif