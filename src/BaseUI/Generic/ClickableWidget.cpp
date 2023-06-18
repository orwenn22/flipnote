#include "ClickableWidget.h"

#include <SDL.h>

#include "../../Core/RunState.h"
#include "../../Globals.h"


ClickableWidget::ClickableWidget(
        WidgetContainer* container,
        int x, int y, int w, int h ,
        WidgetAllign allignment,
        ClickabbleWidgetCallback callback
) 
: Widget(container, x, y, w, h, allignment) {
    m_callback = callback;
}

ClickableWidget::~ClickableWidget() {
    
}


void ClickableWidget::Update() {
    UpdatePos();

    if(g_runstate->mouseused) return;

    if(IsOvered()) {
        g_runstate->mouseused = true;
    
        if(g_runstate->leftclick) {
            m_callback();
        }
    }
}

void ClickableWidget::Render() {
    SDL_FRect dest = {(float)GetX(), (float)GetY(), (float)m_w, (float)m_h};
    SDL_SetRenderDrawColor(g_runstate->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(g_runstate->renderer, &dest);
}
