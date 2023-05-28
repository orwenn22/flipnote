#include "EditorButton.h"

#include "../../Core/RunState.h"
#include "../../Globals.h"

#include <SDL.h>

EditorButton::EditorButton(
    WidgetContainer* container,
    FlipnoteEditor* editor, 
    int x, int y, int w, int h, 
    WidgetAllign allignment, 
    EditorButtonCallback callback
) 
: Widget(container, x, y, w, h, allignment) {
    m_editor = editor;
    m_callback = callback;
}

EditorButton::~EditorButton() {
    
}

void EditorButton::Update() {
    UpdatePos();

    if(g_runstate->mouseused) return;

    if(IsOvered()) {
        g_runstate->mouseused = true;
    
        if(g_runstate->leftclick) {
            m_callback(this);
        }
    }
}

void EditorButton::Render() {
    SDL_FRect dest = {(float)GetX(), (float)GetY(), (float)m_w, (float)m_h};
    SDL_SetRenderDrawColor(g_runstate->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(g_runstate->renderer, &dest);
}
