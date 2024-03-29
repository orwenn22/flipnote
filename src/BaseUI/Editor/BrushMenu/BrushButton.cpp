#include "BrushButton.h"

#include <SDL3/SDL.h>

#include "../../../Core/FlipnoteRessources.h"
#include "../../../Reusable/RunState.h"
#include "../FlipnoteEditor.h"

BrushButton::BrushButton(FlipnoteEditor* editor, int brushindex, int x, int y) : ClickableWidget(x, y) {
    m_editor = editor;
    m_brushindex = brushindex;
    
    m_w = 26;
    m_h = 26;
    m_displayedbrushinverted = false;

    m_brushtexture = NULL;
    InitTexture();

    m_callback = [&]() -> void { m_editor->SetCurrentBrush(m_brushindex); };
}

BrushButton::~BrushButton() {
    SDL_DestroyTexture(m_brushtexture);
}


void BrushButton::PreUpdate() {
    //Reload the texture if the "invert brush" checkbox is clicked
    if(m_displayedbrushinverted != m_editor->m_invertpaint) {
        InitTexture();
    }
}

void BrushButton::Render() {
    float x = (float)GetX();
    float y = (float)GetY();

    SDL_FRect dest = {x, y, 26, 26};
    SDL_RenderTexture(g_runstate->renderer, m_brushtexture, NULL, &dest);

    if(m_brushindex == m_editor->GetCurrentBrush()) {
        dest = {x-2, y-2, 26+4, 26+4};
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
