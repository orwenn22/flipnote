#include "EditorBrushButton.h"

#include <SDL.h>

#include "../../Core/Flipnote/FlipnotePainter.h"
#include "../../Core/Ressources.h"
#include "../../Core/RunState.h"
#include "../../Core/Tileset.h"
#include "../../Globals.h"
#include "../Generic/CheckBox.h"
#include "../Generic/PopupMenuTop.h"
#include "../Generic/PopupMenu.h"
#include "BrushMenu/BrushButton.h"
#include "BrushMenu/BrushSizeButton.h"
#include "FlipnoteEditor.h"
#include "Menu/MenuAligner.h"

EditorBrushButton::EditorBrushButton(WidgetContainer* container, FlipnoteEditor* editor) 
: EditorButton(container, editor, 10, 95, 48, 48, WidgetAllign::WidgetAllign_Left) {
    
    m_displayedbrushkind = -1;
    m_displayedbrushsize = -1;
    m_displayedbrushinverted = false;

    m_texture = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 32, 32);


    m_callback = [&]() -> void {
        PopupMenu* pm = new PopupMenu(GetX() - 10, GetY(), -319, 200);

        pm->AddWidget(new PopupMenuTop(pm, 98));
        pm->AddWidget(new MenuAligner(pm, this));

        //Add button for all brushes
        for(int i = 0; i < GetBrushCount(); i++) {
            pm->AddWidget(new BrushButton(pm, m_editor, i, 10 + (26+4)*(i%10), 10+ (26+4)*(i/10)));
        }
        pm->AddWidget(new CheckBox(pm, 10, 75, &(m_editor->m_invertpaint), "Invert brush"));


        for(int i = 0; i < 8; i++) {
            pm->AddWidget(new BrushSizeButton(pm, m_editor, i*2, 10 + (32+4)*i, 130));
        }

        m_editor->OpenPopupMenu(pm);
    };
}

EditorBrushButton::~EditorBrushButton() {
    SDL_DestroyTexture(m_texture);
}


void EditorBrushButton::PreUpdate() {
    //check if brush settings were modified
    if(m_displayedbrushkind != m_editor->GetCurrentBrush()
    || m_displayedbrushsize != m_editor->GetBrushSize()
    || m_displayedbrushinverted != m_editor->m_invertpaint) {
        UpdateTexture();
    }
}

void EditorBrushButton::Render() {
    float x = (float)GetX();
    float y = (float)GetY();

    g_ressources->tileset_popupmenu->DrawRectangle(g_runstate->renderer, x, y, x+m_w, y+m_h, g_ressources->rectdata_popupmenu);

    SDL_FRect texturedest = {x+8, y+8, 32.0f, 32.0f};
    SDL_RenderTexture(g_runstate->renderer, m_texture, NULL, &texturedest);
}


void EditorBrushButton::UpdateTexture() {
    //printf("aaa\n");
    m_displayedbrushkind = m_editor->GetCurrentBrush();
    m_displayedbrushsize = m_editor->GetBrushSize();
    m_displayedbrushinverted = m_editor->m_invertpaint;

    //clear texture with white
    SDL_SetRenderTarget(g_runstate->renderer, m_texture);
    SDL_SetRenderDrawColor(g_runstate->renderer, 255, 255, 255, 255);
    SDL_RenderClear(g_runstate->renderer);
    SDL_SetRenderTarget(g_runstate->renderer, NULL);

    FlipnotePainter painter((FlipnoteFrame*)NULL, m_texture, GetBrush(m_displayedbrushkind), m_displayedbrushinverted);
    painter.FillCircle(16, 16, m_displayedbrushsize, 1);    //1 is black
}
