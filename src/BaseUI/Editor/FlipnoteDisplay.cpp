#include "FlipnoteDisplay.h"

#include <SDL.h>
#include <stdio.h>

#include "../../Core/Flipnote/FlipnoteFrame.h"
#include "../../Core/Flipnote/Flipnote.h"
#include "../../Core/RunState.h"
#include "../../Globals.h"
#include "FlipnoteEditor.h"

FlipnoteDisplay::FlipnoteDisplay(SDL_Renderer* renderer, FlipnoteEditor* editor) {
    m_editor = editor;
    m_x = 50;
    m_y = 50;
    m_scale = 1.0f;

    m_followmouse = false;
    m_mouseoffsetx = 0;
    m_mouseoffsety = 0;

    m_currentpagetexture = m_editor->CurrentFrame()->CopyToTexture();
}

FlipnoteDisplay::~FlipnoteDisplay() {
    SDL_DestroyTexture(m_currentpagetexture);
}


void FlipnoteDisplay::Update() {
    //Dragging the canvas
    if(m_followmouse) {
        m_x = g_runstate->mousex - m_mouseoffsetx;
        m_y = g_runstate->mousey - m_mouseoffsety;
    }
    
    ////Don't do anything more if mouse have already been used
    //if(g_runstate->mouseused) return;

    UpdateMouseInput();
}


void FlipnoteDisplay::Render(SDL_Renderer* renderer) {
    SDL_FRect dest = {(float)m_x, (float)m_y, (float)512 * m_scale, (float)384 * m_scale};

    SDL_FRect outline = {dest.x - m_scale, dest.y - m_scale, dest.w + m_scale*2, dest.h + m_scale*2};
    SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);   //gray
    SDL_RenderFillRect(renderer, &outline);
    SDL_RenderTexture(renderer, m_currentpagetexture, NULL, &dest);
}


void FlipnoteDisplay::GetMousePosPixel(int* x, int* y) {
    *x = (int)((g_runstate->mousex - m_x)/m_scale);
    *y = (int)((g_runstate->mousey - m_y)/m_scale);
}

void FlipnoteDisplay::GetMousePosRelative(int* x, int* y) {
    *x = g_runstate->mousex - m_x;
    *y = g_runstate->mousey - m_y;
}


bool FlipnoteDisplay::IsMouseOnDisplay() {
    return (g_runstate->mousex >= m_x && g_runstate->mousey >= m_y
    && g_runstate->mousex < m_x + (int)(512.0f*m_scale) && g_runstate->mousey < m_y + (int)(384.0f*m_scale));
}



////////////////////////
//Private



void FlipnoteDisplay::UpdateMouseInput() {
    //Disable dragging the canvas when right is released
    if(!g_runstate->rightpressed) {
        m_followmouse = false;
    }

    //Don't do anything more if mouse have already been used
    if(g_runstate->mouseused) return;

    //Enable dragging the canvas when right is pressed
    if(g_runstate->rightclick) {
        m_followmouse = true;
        m_mouseoffsetx = g_runstate->mousex - m_x;
        m_mouseoffsety = g_runstate->mousey - m_y;
    }


    if(IsMouseOnDisplay()) {
        if(g_runstate->leftclick) m_editor->m_isdrawing = true;

        HandleZoom();
    }
}



//Load the surface of the current page into the gpu
void FlipnoteDisplay::RefreshTexture(SDL_Renderer* renderer) {
    SDL_DestroyTexture(m_currentpagetexture);
    //m_currentpagetexture = SDL_CreateTextureFromSurface(renderer, m_editor->CurrentFrameSurface());
    m_currentpagetexture = m_editor->CurrentFrame()->CopyToTexture();
}

SDL_Texture* FlipnoteDisplay::GetTexture() {
    return m_currentpagetexture;
}


void FlipnoteDisplay::HandleZoom() {
    if(g_runstate->mousewheel != 0) {
        int relativex, relativey;
        GetMousePosRelative(&relativex, &relativey);

        if(g_runstate->mousewheel > 0) {    //zoom
            m_scale *= 2;
            //Double the distance because we are zooming
            relativex *= 2;
            relativey *= 2;
            //In case someone is dragging and zooming at the same time
            m_mouseoffsetx *= 2;
            m_mouseoffsety *= 2;
        }
        else {  // mousewheel < 0       unzoom
            m_scale /= 2;
            //Divide the distance by 2 because we are unzooming
            relativex /= 2;
            relativey /= 2;
            //In case someone is dragging and zooming at the same time
            m_mouseoffsetx /= 2;
            m_mouseoffsety /= 2;
        }

        //Actually apply the new distance
        m_x = g_runstate->mousex - relativex;
        m_y = g_runstate->mousey - relativey;
    }
}