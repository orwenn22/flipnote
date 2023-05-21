#include "PopupMenuItem.h"

#include "../../Core/RunState.h"
#include "../../Globals.h"
#include "PopupMenu.h"

PopupMenuItem::PopupMenuItem(PopupMenu* popupmenu) {
    m_popupmenu = popupmenu; 
    m_x = 0;
    m_y = 0;
    m_w = 0;
    m_h = 0;
}

PopupMenuItem::~PopupMenuItem() {}

bool PopupMenuItem::IsOvered() {
    //Get mouse pos in popup menu
    int mousex, mousey;
    m_popupmenu->GetRelativeMousePos(&mousex, &mousey);

    //Check if mouse is overring
    return (mousex >= m_x && mousex < m_x+m_w 
         && mousey >= m_y && mousey < m_y+m_h);
}