#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <string>

#include "ClickableWidget.h"

class CheckBox : public ClickableWidget {
    public:
    CheckBox(WidgetContainer* container, int x, int y, bool* target, std::string label = "");

    void Render() override;

    private:
    bool* m_target;
};

#endif