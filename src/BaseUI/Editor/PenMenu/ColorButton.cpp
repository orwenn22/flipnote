#include "ColorButton.h"

#include <SDL3/SDL.h>

#include "../../../Core/Flipnote/Flipnote.h"
#include "../../../Core/FlipnoteRessources.h"
#include "../../../Reusable/gui/WidgetContainer.h"
#include "../../../Reusable/RunState.h"
#include "../FlipnoteEditor.h"

#include <stdio.h>

ColorButton::ColorButton(FlipnoteEditor* editor, int index, int x, int y) : ClickableWidget(x, y) {
    m_editor = editor;
    m_index = index;

    SDL_QueryTexture(g_ressources->txtr_button_smallpen, NULL, NULL, &m_w, &m_h);

    m_callback = [&]() -> void { m_editor->SetCurrentColorIndex(m_index); };
}


void ColorButton::Render() {
    float x = (float)GetX();
    float y = (float)GetY();

    //Custom color based on the index
    SDL_Color c = m_editor->GetFlipnote()->GetColor(m_index);
    SDL_SetTextureColorMod(g_ressources->txtr_button_smallpen, c.r, c.g, c.b);

    //Draw pen button on screen
    SDL_FRect dest = {x, y, (float)m_w, (float)m_h};
    SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_button_smallpen, NULL, &dest);
    
    //Restore colors
    SDL_SetTextureColorMod(g_ressources->txtr_button_smallpen, 255, 255, 255);
}
