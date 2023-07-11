#include "EditorPenButton.h"

#include <SDL.h>

#include "../../../Core/Flipnote/Flipnote.h"
#include "../../../Core/FlipnoteRessources.h"
#include "../../../Reusable/gui/PopupMenuTop.h"
#include "../../../Reusable/gui/PopupMenu.h"
#include "../../../Reusable/RunState.h"
#include "../FlipnoteEditor.h"
#include "../Menu/MenuAligner.h"
#include "../PenMenu/ColorButton.h"

EditorPenButton::EditorPenButton(FlipnoteEditor* editor) : EditorButton(editor, 10, 40) {
    SDL_QueryTexture(g_ressources->txtr_button_pen, NULL, NULL, &m_w, &m_h);
    m_allignment = WidgetAllign::WidgetAllign_Left;
    UpdatePos();


    m_callback = [&]() -> void {
        PopupMenu* pm = new PopupMenu(GetX() - 10, GetY(), -305, 100);
        pm->AddWidget(new PopupMenuTop(50));
        pm->AddWidget(new MenuAligner(this));
        int itemwidth;
        SDL_QueryTexture(g_ressources->txtr_button_smallpen, NULL, NULL, &itemwidth, NULL);

        for(int i = 0; i < 8; i++) {    //TODO : 8 color hardcoded
            pm->AddWidget(new ColorButton(m_editor, i, 10 + i*(itemwidth+3), 10));
        }
            
        m_editor->OpenPopupMenu(pm);
    };
}


void EditorPenButton::Render() {
    float x = (float)GetX();
    float y = (float)GetY();

    //Custom color based on the one selected
    SDL_Color c = m_editor->GetFlipnote()->GetColor(m_editor->GetCurrentColorIndex());
    SDL_SetTextureColorMod(g_ressources->txtr_button_pen, c.r, c.g, c.b);

    //Draw pen button on screen
    SDL_FRect dest = {x, y, (float)m_w, (float)m_h};
    SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_button_pen, NULL, &dest);
    
    //Restore colors
    SDL_SetTextureColorMod(g_ressources->txtr_button_pen, 255, 255, 255);
}
