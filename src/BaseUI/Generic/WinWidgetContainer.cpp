#include "WinWidgetContainer.h"

#include "../../Core/RunState.h"
#include "../../Globals.h"

WinWidgetContainer::WinWidgetContainer() : WidgetContainer() {

}

void WinWidgetContainer::Update() {
    UpdateWidgets();
}

void WinWidgetContainer::Render() {
    RenderWidgets();
}

int WinWidgetContainer::GetX() { return 0; }
int WinWidgetContainer::GetY() { return 0; }

void WinWidgetContainer::GetAbsoluteSize(int* w, int* h) {
    *w = GetAbsoluteWidth();
    *h = GetAbsoluteHeight();
}
int WinWidgetContainer::GetAbsoluteWidth() {
    return g_runstate->winwidth;
}
int WinWidgetContainer::GetAbsoluteHeight() {
    return g_runstate->winheight;
}

bool WinWidgetContainer::IsMouseOvering() {
    return true;    //from our perspective, assume the mouse is always overring the window
}

void WinWidgetContainer::GetRelativeMousePos(int* x, int* y) {
    *x = g_runstate->mousex;
    *y = g_runstate->mousey;
}