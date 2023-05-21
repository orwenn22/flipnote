#include "EditorPenButton.h"

#include <SDL.h>

#include "../../Core/Flipnote/FlipnoteFrame.h"
#include "../../Core/Ressources.h"
#include "../../Core/RunState.h"
#include "../../Globals.h"
#include "../Generic/PopupMenuTop.h"
#include "../Generic/PopupMenu.h"
#include "FlipnoteEditor.h"
#include "PenMenu/ColorButton.h"
#include "PenMenu/PenMenuAligner.h"

EditorPenButton::EditorPenButton(FlipnoteEditor* editor) {
    m_editor = editor;
    SDL_QueryTexture(g_ressources->txtr_penbutton, NULL, NULL, &m_w, &m_h);
    UpdatePos();
}

void EditorPenButton::Update() {
    UpdatePos();

    if(g_runstate->mouseused) return;

    if(g_runstate->mousex >= m_x && g_runstate->mousex < m_x+m_w
    && g_runstate->mousey >= m_y && g_runstate->mousey < m_y+m_h) {
        g_runstate->mouseused = true;
        
        if(g_runstate->leftclick) {
            PopupMenu* pm = new PopupMenu(m_x - 10, m_y, -305, 100);
            pm->AddWidget(new PopupMenuTop(pm, 50));
            pm->AddWidget(new PenMenuAligner(pm, this));
            int itemwidth;
            SDL_QueryTexture(g_ressources->txtr_smallpenbutton, NULL, NULL, &itemwidth, NULL);

            for(int i = 0; i < 8; i++) {    //TODO : 8 color hardcoded
                pm->AddWidget(new ColorButton(pm, m_editor, i, 10 + i*(itemwidth+3), 10));
            }
            
            m_editor->OpenPopupMenu(pm);
        }
    }
}

void EditorPenButton::Render() {
    //Custom color based on the one selected
    SDL_Color c = m_editor->CurrentFrame()->GetColor(m_editor->GetCurrentColorIndex());
    SDL_SetTextureColorMod(g_ressources->txtr_penbutton, c.r, c.g, c.b);

    //Draw pen button on screen
    SDL_FRect dest = {(float)m_x, (float)m_y, (float)m_w, (float)m_h};
    SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_penbutton, NULL, &dest);
    
    //Restore colors
    SDL_SetTextureColorMod(g_ressources->txtr_penbutton, 255, 255, 255);
}


int EditorPenButton::GetX() { return m_x; }

void EditorPenButton::UpdatePos() {
    //Top tight of window.
    m_x = g_runstate->winwidth-m_w-10;
    m_y = 40;
}