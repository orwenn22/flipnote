#include "MenuAligner.h"

#include "../../../Reusable/gui/PopupMenu.h"
#include "../EditorButtons/EditorButton.h"

MenuAligner::MenuAligner(WidgetContainer* container, EditorButton* editorbutton) : Widget(container, 0, 0) {
    m_editorbutton = editorbutton;
}

void MenuAligner::Update() {
    ((PopupMenu*)(m_container))->SetX(m_editorbutton->GetX() - 10);
}

void MenuAligner::Render() {}
