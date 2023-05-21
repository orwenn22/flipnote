#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <string>

#include "PopupMenuItem.h"

class CheckBox : public PopupMenuItem {
    public:
    CheckBox(PopupMenu* popupmenu, int x, int y, bool* target, std::string label = "");

    void Update();
    void Render();

    private:
    bool* m_target;
};

#endif