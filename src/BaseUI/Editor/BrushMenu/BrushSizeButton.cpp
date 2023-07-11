#include "BrushSizeButton.h"

#include <SDL.h>

#include "../../../Core/Flipnote/FlipnotePainter.h"
#include "../../../Core/FlipnoteRessources.h"
#include "../../../Reusable/gui/WidgetContainer.h"
#include "../../../Reusable/RunState.h"
#include "../FlipnoteEditor.h"

BrushSizeButton::BrushSizeButton(FlipnoteEditor* editor, int brushsize, int x, int y) : ClickableWidget(x, y) {
    m_w = 32;
    m_h = 32;

    m_editor = editor;
    m_brushsize = brushsize;

    m_texture = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 32, 32);

    FlipnotePainter painter(m_texture);
    painter.FillCircle(m_w/2, m_h/2, brushsize, 1); //1 is black on the default palette

    m_callback = [&]() -> void { m_editor->SetBrushSize(m_brushsize); };
}

BrushSizeButton::~BrushSizeButton() {
    SDL_DestroyTexture(m_texture);
}

void BrushSizeButton::Render() {
    float x = (float)GetX();
    float y = (float)GetY();
    SDL_FRect dest = {x, y, (float)m_w, (float)m_h};
    SDL_RenderTexture(g_runstate->renderer, m_texture, NULL, &dest);


    if(m_editor->GetBrushSize() == m_brushsize) {
        dest = {x-2, y-2, (float)m_w+4, (float)m_h+4};
        SDL_RenderRect(g_runstate->renderer, &dest);
    }
}
