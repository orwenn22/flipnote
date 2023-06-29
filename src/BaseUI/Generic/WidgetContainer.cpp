#include "WidgetContainer.h"

#include <algorithm>
#include <stddef.h>

#include "Widget.h"

WidgetContainer::WidgetContainer() {
}

WidgetContainer::~WidgetContainer() {
    Clear();
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


void WidgetContainer::RemoveWidget(Widget* widget) {
    auto pos = std::find(m_widgets.begin(), m_widgets.end(), widget);
    if(pos == m_widgets.end()) {
        return;
    }
    m_widgets.erase(pos);
}

void WidgetContainer::Clear() {
    for(Widget* i : m_widgets) {
        delete i;
    }
    m_widgets.clear();
}


void WidgetContainer::GetAbsoluteSize(int* w, int* h) {
    *w = GetContainerWidth();
    *h = GetContainerHeight();
}