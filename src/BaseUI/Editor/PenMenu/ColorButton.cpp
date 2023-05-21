#include "ColorButton.h"

#include <SDL.h>

#include "../../../Globals.h"
#include "../../../Core/Flipnote/FlipnoteFrame.h"
#include "../../../Core/Ressources.h"
#include "../../../Core/RunState.h"
#include "../../Generic/PopupMenu.h"
#include "../FlipnoteEditor.h"

#include <stdio.h>

ColorButton::ColorButton(PopupMenu* popupmenu, FlipnoteEditor* editor, int index, int x, int y) : PopupMenuItem(popupmenu) {
    m_editor = editor;
    m_index = index;

    m_x = x;
    m_y = y;
    SDL_QueryTexture(g_ressources->txtr_smallpenbutton, NULL, NULL, &m_w, &m_h);
}

void ColorButton::Update() {
    //This is for when the menu is not fully deployed (the animation is not over)
    if(!m_popupmenu->IsMouseOvering()) return;

    ////Get mouse pos in popup menu
    //int mousex, mousey;
    //m_popupmenu->GetRelativeMousePos(&mousex, &mousey);

    if(g_runstate->mouseused) return;

    //Check if mouse is overring
    if(IsOvered()) {
        if(g_runstate->leftclick) {
            m_editor->SetCurrentColorIndex(m_index);
        }
    }
}

void ColorButton::Render() {
    //Custom color based on the index
    SDL_Color c = m_editor->CurrentFrame()->GetColor(m_index);
    SDL_SetTextureColorMod(g_ressources->txtr_smallpenbutton, c.r, c.g, c.b);

    //Draw pen button on screen
    SDL_FRect dest = {(float)m_x, (float)m_y, (float)m_w, (float)m_h};
    SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_smallpenbutton, NULL, &dest);
    
    //Restore colors
    SDL_SetTextureColorMod(g_ressources->txtr_smallpenbutton, 255, 255, 255);
}