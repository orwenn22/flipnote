#include "WidgetContainer.h"

#include <stddef.h>

#include "Widget.h"

WidgetContainer::WidgetContainer(WidgetContainer* parrent) {
    m_parrent = parrent;
}

WidgetContainer::~WidgetContainer() {
    for(Widget* i : m_widgets) {
        delete i;
    }
}


void WidgetContainer::GetPosition(int* x, int* y) {
    *x = GetX();
    *y = GetY();
}

int WidgetContainer::GetAbsoluteX() {
     if(m_parrent == NULL)
        return GetX();
    else
        return GetX() + m_parrent->GetAbsoluteX();
}

int WidgetContainer::GetAbsoluteY() {
    if(m_parrent == NULL)
        return GetY();
    else
        return GetY() + m_parrent->GetAbsoluteY();
}

void WidgetContainer::GetAbsolutePosition(int* x, int* y) {
    *x = GetAbsoluteX();
    *y = GetAbsoluteY();
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