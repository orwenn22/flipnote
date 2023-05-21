#include "BrushSizeButton.h"

#include <SDL.h>

#include "../../../Core/Flipnote/FlipnotePainter.h"
#include "../../../Core/Ressources.h"
#include "../../../Core/RunState.h"
#include "../../../Globals.h"
#include "../../Generic/PopupMenu.h"
#include "../FlipnoteEditor.h"

BrushSizeButton::BrushSizeButton(PopupMenu* popupmenu, FlipnoteEditor* editor, int brushsize, int x, int y) : PopupMenuItem(popupmenu) {
    m_x = x;
    m_y = y;
    m_w = 32;
    m_h = 32;

    m_editor = editor;
    m_brushsize = brushsize;

    m_texture = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 32, 32);

    FlipnotePainter painter(NULL, m_texture);
    painter.FillCircle(m_w/2, m_h/2, brushsize, 1);
}

BrushSizeButton::~BrushSizeButton() {
    SDL_DestroyTexture(m_texture);
}

void BrushSizeButton::Update() {
    //This is for when the menu is not fully deployed (the animation is not over)
    if(!m_popupmenu->IsMouseOvering()) return;
    if(g_runstate->mouseused) return;

    if(IsOvered() && g_runstate->leftclick) {
        m_editor->SetBrushSize(m_brushsize);
    }
}

void BrushSizeButton::Render() {
    SDL_FRect dest = {(float)m_x, (float)m_y, (float)m_w, (float)m_h};
    SDL_RenderTexture(g_runstate->renderer, m_texture, NULL, &dest);


    if(m_editor->GetBrushSize() == m_brushsize) {
        dest = {(float)m_x-2, (float)m_y-2, (float)m_w+4, (float)m_h+4};
        SDL_RenderRect(g_runstate->renderer, &dest);
    }
}