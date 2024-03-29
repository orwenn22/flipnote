#include "EditorPlayButton.h"

#include "../../../Core/FlipnoteRessources.h"
#include "../../../Reusable/gui/WidgetContainer.h"
#include "../../../Reusable/RunState.h"
#include "../FlipnoteEditor.h"

#include <SDL3/SDL.h>

EditorPlayButton::EditorPlayButton(
    FlipnoteEditor* editor,
    int x, int y, WidgetAllign allignment
) : EditorButton(editor, x, y, 50, 50, allignment) {
    m_callback = [&]() -> void {
        m_editor->m_animmationplaying = !m_editor->m_animmationplaying;
    };
}


void EditorPlayButton::Render() {
    float x = (float)GetX();
    float y = (float)GetY();
    SDL_FRect dest = {x, y, (float)m_w, (float)m_h};

    if(m_editor->m_animmationplaying) {
        SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_button_pause, NULL, &dest);
    }
    else {
        SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_button_play, NULL, &dest);
    }
}
