#include "EditorPlayButton.h"

#include "../../../Core/Ressources.h"
#include "../../../Reusable/RunState.h"
#include "../../../Globals.h"
#include "../../Generic/WidgetContainer.h"
#include "../FlipnoteEditor.h"

#include <SDL.h>

EditorPlayButton::EditorPlayButton(
    WidgetContainer* container, FlipnoteEditor* editor,
    int x, int y, WidgetAllign allignment
) : EditorButton(container, editor, x, y, 50, 50, allignment) {
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
