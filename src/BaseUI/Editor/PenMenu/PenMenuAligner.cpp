#include "PenMenuAligner.h"

#include "../../Generic/PopupMenu.h"
#include "../EditorPenButton.h"

PenMenuAligner::PenMenuAligner(WidgetContainer* container, EditorPenButton* penbutton) : Widget(container) {
    m_penbutton = penbutton;
}

void PenMenuAligner::Update() {
    ((PopupMenu*)(m_container))->SetX(m_penbutton->GetX() - 10);
}

void PenMenuAligner::Render() {}