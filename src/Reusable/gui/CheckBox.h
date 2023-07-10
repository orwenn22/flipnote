#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <string>

#include "ClickableWidget.h"

class Label;

class CheckBox : public ClickableWidget {
    public:
    CheckBox(int x, int y, bool* target, std::string label = "", WidgetAllign allignment = WidgetAllign_None);
    ~CheckBox();

    void Render() override;

    protected:
    void OnContainerAdd() override;

    private:
    bool* m_target;
    Label* m_label;
};

#endif