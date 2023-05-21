#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <string>

#include "Widget.h"

class CheckBox : public Widget {
    public:
    CheckBox(WidgetContainer* container, int x, int y, bool* target, std::string label = "");

    void Update();
    void Render();

    private:
    bool* m_target;
};

#endif