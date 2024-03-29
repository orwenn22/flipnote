#include "EditorTimelineButton.h"

#include "../../../Core/FlipnoteRessources.h"
#include "../../../Reusable/RunState.h"
#include "../../../Reusable/Tileset.h"
#include "../FlipnoteEditor.h"

#include <SDL3/SDL.h>

EditorTimelineButton::EditorTimelineButton(
    FlipnoteEditor* editor,
    int x, int y, WidgetAllign allignment
) : EditorButton(editor, x, y, 50, 50, allignment) {
    m_callback = [&]() -> void {
        if(m_editor->IsTimelineOpen()) m_editor->CloseTimeline();
        else m_editor->OpenTimeline();
    };
}


void EditorTimelineButton::Render() {
    float x = (float)GetX();
    float y = (float)GetY();

    SDL_FRect dest = {x, y, (float)m_w, (float)m_h};
    g_ressources->tileset_popupmenu->DrawRectangle(g_runstate->renderer, &dest, g_ressources->rectdata_popupmenu);

    dest.x += 1.0f;
    dest.y += 1.0f;
    dest.w -= 2.0f;
    dest.h -= 2.0f;
    SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_button_timeline, NULL, &dest);
}
