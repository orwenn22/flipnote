#include "MenuAligner.h"

#include "../../Generic/PopupMenu.h"
#include "../EditorButton.h"

MenuAligner::MenuAligner(WidgetContainer* container, EditorButton* editorbutton) : Widget(container, 0, 0) {
    m_editorbutton = editorbutton;
}

void MenuAligner::Update() {
    ((PopupMenu*)(m_container))->SetX(m_editorbutton->GetX() - 10);
}

void MenuAligner::Render() {}