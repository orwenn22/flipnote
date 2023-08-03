#include "FullscreenPopup.h"

#include <SDL3/SDL.h>

#include "../Ressources.h"
#include "../RunState.h"
#include "../Tileset.h"


FullscreenPopup::FullscreenPopup(int w, int h) {
    m_width = w;
    m_height = h;
    m_texture = nullptr;

    if(m_width > 0 && m_height > 0) {
        m_texture = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, m_width, m_height);
        SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
    }
}

FullscreenPopup::~FullscreenPopup() {
    if(m_texture != nullptr) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}

void FullscreenPopup::Update() {
    if(m_texture != nullptr) {
        UpdateWidgets();
        g_runstate->mouseused = true;
    }
}

void FullscreenPopup::Render() {
    if(m_texture != nullptr) {
        SDL_SetRenderDrawBlendMode(g_runstate->renderer, SDL_BLENDMODE_BLEND);

        //Draw the widgets on the texture
        SDL_Texture* previous_render_target = SDL_GetRenderTarget(g_runstate->renderer);
        SDL_SetRenderTarget(g_runstate->renderer, m_texture);
        SDL_SetRenderDrawColor(g_runstate->renderer, 0, 0, 0, 0);
        SDL_RenderClear(g_runstate->renderer);
        RenderWidgets();
        SDL_SetRenderTarget(g_runstate->renderer, previous_render_target);

        if(previous_render_target != NULL) {
            printf("FullscreenPopup::Render : trying to render on something other than the main viewport\n");
            //return
        }

        //Calculate the position of the popup on the screen (can't be a custom render target for now)
        int rw, rh;
        SDL_GetRenderOutputSize(g_runstate->renderer, &rw, &rh);    //TODO : adapt this for drawing on custom render targets ?
        int x = rw/2 - m_width/2;
        int y = rh/2 - m_height/2;
        SDL_FRect dest = {(float)x, (float)y, (float)m_width, (float)m_height};

        //Dark background
        SDL_SetRenderDrawColor(g_runstate->renderer, 0, 0, 0, 128);
        SDL_RenderFillRect(g_runstate->renderer, nullptr);

        //Content
        g_reusableressources->tileset_popupmenu->DrawRectangle(g_runstate->renderer, &dest, g_reusableressources->rectdata_popupmenu);
        SDL_RenderTexture(g_runstate->renderer, m_texture, nullptr, &dest);
    }
}

int FullscreenPopup::GetContainerX() {
    int rw;
    SDL_GetRenderOutputSize(g_runstate->renderer, &rw, nullptr);
    return rw/2 - m_width/2;
}

int FullscreenPopup::GetContainerY() {
    int rh;
    SDL_GetRenderOutputSize(g_runstate->renderer, nullptr, &rh);
    return rh/2 - m_height/2;
}

int FullscreenPopup::GetContainerWidth() {
    return m_width;
}

int FullscreenPopup::GetContainerHeight() {
    return m_height;
}

bool FullscreenPopup::IsMouseOvering() {
    //TODO : adapt this for drawing on custom render targets ?
    int mx = (int)g_runstate->mousex;
    int my = (int)g_runstate->mousey;

    int cx = GetContainerX();
    int cy = GetContainerY();

    return mx >= cx && my >= cy && mx < cx+m_width && my < cy+m_height;
}

void FullscreenPopup::GetRelativeMousePos(int *x, int *y) {
    *x = (int)g_runstate->mousex - GetContainerX();
    *y = (int)g_runstate->mousey - GetContainerY();
}
