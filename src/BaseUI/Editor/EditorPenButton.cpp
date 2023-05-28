#include "EditorPenButton.h"

#include <SDL.h>

#include "../../Core/Flipnote/FlipnoteFrame.h"
#include "../../Core/Ressources.h"
#include "../../Core/RunState.h"
#include "../../Globals.h"
#include "../Generic/PopupMenuTop.h"
#include "../Generic/PopupMenu.h"
#include "FlipnoteEditor.h"
#include "Menu/MenuAligner.h"
#include "PenMenu/ColorButton.h"

EditorPenButton::EditorPenButton(WidgetContainer* container, FlipnoteEditor* editor) : EditorButton(container, editor, 10, 40) {
    SDL_QueryTexture(g_ressources->txtr_penbutton, NULL, NULL, &m_w, &m_h);
    m_allignment = WidgetAllign::WidgetAllign_Left;
    UpdatePos();


    m_callback = [&](EditorButton*) -> void {
        PopupMenu* pm = new PopupMenu(GetX() - 10, GetY(), -305, 100);
        pm->AddWidget(new PopupMenuTop(pm, 50));
        pm->AddWidget(new MenuAligner(pm, this));
        int itemwidth;
        SDL_QueryTexture(g_ressources->txtr_smallpenbutton, NULL, NULL, &itemwidth, NULL);

        for(int i = 0; i < 8; i++) {    //TODO : 8 color hardcoded
            pm->AddWidget(new ColorButton(pm, m_editor, i, 10 + i*(itemwidth+3), 10));
        }
            
        m_editor->OpenPopupMenu(pm);
    };
}


void EditorPenButton::Render() {
    float x = (float)GetX();
    float y = (float)GetY();

    //Custom color based on the one selected
    SDL_Color c = m_editor->CurrentFrame()->GetColor(m_editor->GetCurrentColorIndex());
    SDL_SetTextureColorMod(g_ressources->txtr_penbutton, c.r, c.g, c.b);

    //Draw pen button on screen
    SDL_FRect dest = {x, y, (float)m_w, (float)m_h};
    SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_penbutton, NULL, &dest);
    
    //Restore colors
    SDL_SetTextureColorMod(g_ressources->txtr_penbutton, 255, 255, 255);
}