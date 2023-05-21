#include "Widget.h"

#include "../../Core/RunState.h"
#include "../../Globals.h"
#include "WidgetContainer.h"

Widget::Widget(WidgetContainer* container) {
    m_container = container; 
    m_x = 0;
    m_y = 0;
    m_w = 0;
    m_h = 0;
}

Widget::~Widget() {}

bool Widget::IsOvered() {
    //Get mouse pos in popup menu
    int mousex, mousey;
    m_container->GetRelativeMousePos(&mousex, &mousey);

    //Check if mouse is overring
    return (mousex >= m_x && mousex < m_x+m_w 
         && mousey >= m_y && mousey < m_y+m_h);
}