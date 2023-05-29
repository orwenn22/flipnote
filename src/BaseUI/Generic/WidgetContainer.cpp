#include "WidgetContainer.h"

#include <stddef.h>

#include "Widget.h"

WidgetContainer::WidgetContainer() {
}

WidgetContainer::~WidgetContainer() {
    for(Widget* i : m_widgets) {
        delete i;
    }
}


void WidgetContainer::UpdateWidgets() {
    for(int i = m_widgets.size()-1; i >= 0; i--) m_widgets[i]->UpdatePos();
    for(int i = m_widgets.size()-1; i >= 0; i--) m_widgets[i]->Update();
}

void WidgetContainer::RenderWidgets() {
    for(Widget* i : m_widgets) i->Render();
}

void WidgetContainer::AddWidget(Widget* widget) {
    if(widget != NULL) m_widgets.push_back(widget);
}

void WidgetContainer::GetAbsoluteSize(int* w, int* h) {
    *w = GetAbsoluteWidth();
    *h = GetAbsoluteHeight();
}