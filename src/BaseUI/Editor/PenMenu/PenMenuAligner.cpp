#include "PenMenuAligner.h"

#include "../../Generic/PopupMenu.h"
#include "../EditorPenButton.h"

PenMenuAligner::PenMenuAligner(PopupMenu* popupmenu, EditorPenButton* penbutton) : Widget(popupmenu) {
    m_penbutton = penbutton;
}

void PenMenuAligner::Update() {
    m_popupmenu->SetX(m_penbutton->GetX() - 10);
}

void PenMenuAligner::Render() {}