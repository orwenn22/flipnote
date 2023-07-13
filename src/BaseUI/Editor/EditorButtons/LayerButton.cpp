#include "LayerButton.h"

#include <SDL.h>

#include "../../../Core/FlipnoteRessources.h"
#include "../../../Reusable/RunState.h"
#include "../FlipnoteEditor.h"


LayerButton::LayerButton(
    FlipnoteEditor* editor, int layerindex,
    int x, int y, int w, int h,
    WidgetAllign allignment
) : EditorButton(editor, x, y, w, h, allignment) {
    m_layerindex = layerindex;
    m_callback = [&]() { m_editor->m_targetlayer = m_layerindex; };
}

void LayerButton::Render() {
    SDL_FRect dest = {(float)GetX(), (float)GetY(), (float)m_w, (float)m_h};
    if(m_editor->m_targetlayer == m_layerindex) {
        SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_button_layer_active, NULL, &dest);
    }
    else {
        SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_button_layer_inactive, NULL, &dest);
    }
}
