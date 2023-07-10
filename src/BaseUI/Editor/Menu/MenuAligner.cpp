#include "MenuAligner.h"

#include "../../../Reusable/gui/PopupMenu.h"
#include "../EditorButtons/EditorButton.h"

MenuAligner::MenuAligner(WidgetContainer* container, EditorButton* editorbutton, int xallignoffset, int yallignoffset) : Widget(container, 0, 0) {
    m_editorbutton = editorbutton;
    m_xallignoffset = xallignoffset;
    m_yallignoffset = yallignoffset;
}

void MenuAligner::Update() {
    ((PopupMenu*)(m_container))->SetX(m_editorbutton->GetX() + m_xallignoffset);
    ((PopupMenu*)(m_container))->SetY(m_editorbutton->GetY() + m_yallignoffset);
}

void MenuAligner::Render() {}
