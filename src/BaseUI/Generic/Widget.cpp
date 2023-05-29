#include "Widget.h"

#include "../../Core/RunState.h"
#include "../../Globals.h"
#include "WidgetContainer.h"

Widget::Widget(WidgetContainer* container, int x, int y, int w, int h, WidgetAllign allignment) {
    m_container = container; 
    m_xoffset = x;
    m_yoffset = y;
    m_w = w;
    m_h = h;
    m_allignment = allignment;
    UpdatePos();
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

int Widget::GetX() { return m_x; }
int Widget::GetY() { return m_y; }

void Widget::GetPos(int* x, int* y) {
    *x = m_x;
    *y = m_y;
}


int Widget::GetAbsoluteX() { 
    //return GetX() + m_container->GetContainerX();
    int x = GetX();
    if(m_container != NULL) x += m_container->GetContainerX();
    return x;
    
}
int Widget::GetAbsoluteY() { 
    //return GetY() + m_container->GetContainerY();
    int y = GetY();
    if(m_container != NULL) y += m_container->GetContainerY();
    return y;
}

void Widget::GetAbsolutePosition(int* x, int* y) {
    *x = GetAbsoluteX();
    *y = GetAbsoluteY();
}



void Widget::UpdatePos() {
    if(m_allignment == WidgetAllign::WidgetAllign_None) {
        m_x = m_xoffset;
        m_y = m_yoffset;
        return;
    }

    if((m_allignment & WidgetAllign::WidgetAllign_Left) != 0) {
        m_x = m_container->GetContainerWidth() - m_xoffset - m_w;
    }
    else {
        m_x = m_xoffset;
    }

    if((m_allignment & WidgetAllign::WidgetAllign_Bottom) != 0) {
        m_y = m_container->GetContainerHeight() - m_yoffset - m_h;
    }
    else {
        m_y = m_yoffset;
    }
}
