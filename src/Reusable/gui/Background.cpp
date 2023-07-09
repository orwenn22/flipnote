#include "Background.h"

#include <SDL.h>

#include <stdio.h>

#include "../../Reusable/RunState.h"

Background::Background(SDL_Texture* tiletexture) {
    m_bgtexture = NULL;
    m_tiletexture = tiletexture;
    SDL_QueryTexture(m_tiletexture, NULL, NULL, &m_tilewidth, &m_tileheight);

    UpdateTexture();
}

Background::~Background() {
    SDL_DestroyTexture(m_bgtexture);
}

void Background::Update() {
    if(g_runstate->winwidth != m_width || g_runstate->winheight != m_height) {
        UpdateTexture();
    }

}

void Background::Render() {
    SDL_FRect src = {0, 0, (float)m_width, (float)m_height};
    SDL_RenderTexture(g_runstate->renderer, m_bgtexture, &src, &src);
}


void Background::UpdateTexture() {
    m_width = g_runstate->winwidth;
    m_height = g_runstate->winheight;

    if(m_bgtexture != NULL) {
        SDL_DestroyTexture(m_bgtexture);
        m_bgtexture = NULL;
    }

    m_bgtexture = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, m_width, m_height);
    SDL_SetRenderTarget(g_runstate->renderer, m_bgtexture);     //Draw on the texture

    for(int y = 0; y < m_height; y+=m_tileheight) {
        for(int x = 0; x < m_width; x+=m_tilewidth) {
            SDL_FRect dest = {(float)x, (float)y, (float)m_tilewidth, (float)m_tileheight};
            SDL_RenderTexture(g_runstate->renderer, m_tiletexture, NULL, &dest);
        }
    }

    SDL_SetRenderTarget(g_runstate->renderer, NULL);    //NULL is for window
}