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

EditorBrushButton::EditorBrushButton(FlipnoteEditor* editor) {
    m_editor = editor;
    m_w = 48;
    m_h = 48;
    UpdatePos();

    m_displayedbrushkind = -1;
    m_displayedbrushsize = -1;
    m_displayedbrushinverted = false;

    m_texture = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 32, 32);
}

EditorBrushButton::~EditorBrushButton() {
    SDL_DestroyTexture(m_texture);
}

void EditorBrushButton::Update() {
    //update the button's position if the window got resized
    UpdatePos();

    //check if brush settings were modified
    if(m_displayedbrushkind != m_editor->GetCurrentBrush()
    || m_displayedbrushsize != m_editor->GetBrushSize()
    || m_displayedbrushinverted != m_editor->m_invertpaint) {
        UpdateTexture();
    }

    if(g_runstate->mouseused) return;

    if(g_runstate->mousex >= m_x && g_runstate->mousex < m_x+m_w
    && g_runstate->mousey >= m_y && g_runstate->mousey < m_y+m_h) {
        g_runstate->mouseused = true;
        
        if(g_runstate->leftclick) {
            PopupMenu* pm = new PopupMenu(m_x - 10, m_y, -319, 200);

            pm->AddItem(new PopupMenuTop(pm, 98));

            //Add button for all brushes
            for(int i = 0; i < GetBrushCount(); i++) {
                pm->AddItem(new BrushButton(pm, m_editor, i, 10 + (26+4)*(i%10), 10+ (26+4)*(i/10)));
            }
            pm->AddItem(new CheckBox(pm, 10, 75, &(m_editor->m_invertpaint), "Invert brush"));


            for(int i = 0; i < 8; i++) {
                pm->AddItem(new BrushSizeButton(pm, m_editor, i*2, 10 + (32+4)*i, 130));
            }

            m_editor->OpenPopupMenu(pm);
        }
    }
}

void EditorBrushButton::Render() {
    g_ressources->tileset_popupmenu->DrawRectangle(g_runstate->renderer, m_x, m_y, m_x+m_w, m_y+m_h, g_ressources->rectdata_popupmenu);

    SDL_FRect texturedest = {(float)m_x+8, (float)m_y+8, 32.0f, 32.0f};
    SDL_RenderTexture(g_runstate->renderer, m_texture, NULL, &texturedest);
}


int EditorBrushButton::GetX() { return m_x; }

void EditorBrushButton::UpdatePos() {
    //Top tight of window.
    m_x = g_runstate->winwidth-m_w-10;
    m_y = 95;
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

    FlipnotePainter painter(NULL, m_texture, GetBrush(m_displayedbrushkind), m_displayedbrushinverted);
    painter.FillCircle(16, 16, m_displayedbrushsize, 1);    //1 is black
}