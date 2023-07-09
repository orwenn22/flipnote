#include "FrogMenuButton.h"

#include <SDL.h>

#include "../../../Core/FlipnoteRessources.h"
#include "../../../Core/Globals.h"
#include "../../../Reusable/RunState.h"

#include <stdio.h>

FrogMenuButton::FrogMenuButton(
    WidgetContainer* container, FlipnoteEditor* editor,
    int x, int y, WidgetAllign allignment
) : EditorButton(container, editor, x, y, 0, 0, allignment) {
    SDL_QueryTexture(g_ressources->txtr_button_frog, NULL, NULL, &m_w, &m_h);

    m_callback = [&]() { printf("FrogMenuButton clicked\n"); };
}

void FrogMenuButton::Render() {
    SDL_FRect dest = {(float)GetX(), (float)GetY(), m_w, m_h};
    SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_button_frog, NULL, &dest);
}
