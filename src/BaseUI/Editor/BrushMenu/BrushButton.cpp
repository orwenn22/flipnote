#include "BrushButton.h"

#include <SDL.h>

#include "../../../Core/Ressources.h"
#include "../../../Core/RunState.h"
#include "../../../Globals.h"
#include "../../Generic/PopupMenu.h"
#include "../FlipnoteEditor.h"

BrushButton::BrushButton(PopupMenu* popupmenu, FlipnoteEditor* editor, int brushindex, int x, int y) : PopupMenuItem(popupmenu) {
    m_editor = editor;
    m_brushindex = brushindex;
    m_x = x;
    m_y = y;
    m_w = 26;
    m_h = 26;
    m_displayedbrushinverted = false;

    m_brushtexture = NULL;
    InitTexture();

}

BrushButton::~BrushButton() {
    SDL_DestroyTexture(m_brushtexture);
}


void BrushButton::Update() {
    if(m_displayedbrushinverted != m_editor->m_invertpaint) {
        InitTexture();
    }

    //This is for when the menu is not fully deployed (the animation is not over)
    if(!m_popupmenu->IsMouseOvering()) return;

    if(g_runstate->mouseused) return;

    if(IsOvered() && g_runstate->leftclick) {
        m_editor->SetCurrentBrush(m_brushindex);
    }
}

void BrushButton::Render() {
    SDL_FRect dest = {(float)m_x, (float)m_y, 26, 26};
    SDL_RenderTexture(g_runstate->renderer, m_brushtexture, NULL, &dest);

    if(m_brushindex == m_editor->GetCurrentBrush()) {
        dest = {(float)m_x-2, (float)m_y-2, 26+4, 26+4};
        SDL_Color* orange = g_ressources->col_orange;
        SDL_SetRenderDrawColor(g_runstate->renderer, orange->r, orange->g, orange->b, 255);
        SDL_RenderRect(g_runstate->renderer, &dest);
    }
}


void BrushButton::InitTexture() {
    if(m_brushtexture != NULL) SDL_DestroyTexture(m_brushtexture);

    m_displayedbrushinverted = m_editor->m_invertpaint;

    m_brushtexture = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 13, 13);
    PaintCondition brush = GetBrush(m_brushindex);

    SDL_SetRenderTarget(g_runstate->renderer, m_brushtexture);

    SDL_SetRenderDrawColor(g_runstate->renderer, 0, 0, 0, 255);
    for(int cy = 0; cy<13; cy++) 
        for(int cx = 0; cx<13; cx++)
            if(brush(cx,cy) ^ m_editor->m_invertpaint)
                SDL_RenderPoint(g_runstate->renderer, cx, cy);

    SDL_SetRenderTarget(g_runstate->renderer, NULL);
}
