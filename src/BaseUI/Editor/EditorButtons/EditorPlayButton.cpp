#include "EditorPlayButton.h"

#include "../../../Core/Ressources.h"
#include "../../../Core/RunState.h"
#include "../../../Globals.h"
#include "../FlipnoteEditor.h"

#include <SDL.h>

EditorPlayButton::EditorPlayButton(WidgetContainer* container, FlipnoteEditor* editor) 
: EditorButton(container, editor, 65, 10, 50, 50, WidgetAllign::WidgetAllign_BottomLeft) {
    m_callback = [&]() -> void {
        m_editor->m_animmationplaying = !m_editor->m_animmationplaying;
    };
}


void EditorPlayButton::Render() {
    float x = (float)GetX();
    float y = (float)GetY();
    SDL_FRect dest = {x, y, (float)m_w, (float)m_h};

    if(m_editor->m_animmationplaying) {
        SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_pausebutton, NULL, &dest);
    }
    else {
        SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_playbutton, NULL, &dest);
    }
}
