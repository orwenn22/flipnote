#include "Background.h"

#include <SDL.h>

#include <stdio.h>

#include "../../Core/Ressources.h"
#include "../../Reusable/RunState.h"
#include "../../Globals.h"

Background::Background(SDL_Renderer* renderer) {
    m_bgtexture = NULL;
    SDL_QueryTexture(g_ressources->txtr_bgtile, NULL, NULL, &m_tilewidth, &m_tileheight);

    UpdateTexture(renderer);
}

Background::~Background() {
    SDL_DestroyTexture(m_bgtexture);
}

void Background::Update(SDL_Renderer* renderer) {
    if(g_runstate->winwidth != m_width || g_runstate->winheight != m_height) {
        UpdateTexture(renderer);
    }

}

void Background::Render(SDL_Renderer* renderer) {
    SDL_FRect src = {0, 0, (float)m_width, (float)m_height};
    SDL_RenderTexture(renderer, m_bgtexture, &src, &src);
}


void Background::UpdateTexture(SDL_Renderer* renderer) {
    m_width = g_runstate->winwidth;
    m_height = g_runstate->winheight;

    if(m_bgtexture != NULL) {
        SDL_DestroyTexture(m_bgtexture);
        m_bgtexture = NULL;
    }

    m_bgtexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, m_width, m_height);
    SDL_SetRenderTarget(renderer, m_bgtexture);     //Draw on the texture

    for(int y = 0; y < m_height; y+=m_tileheight) {
        for(int x = 0; x < m_width; x+=m_tilewidth) {
            SDL_FRect dest = {(float)x, (float)y, (float)m_tilewidth, (float)m_tileheight};
            SDL_RenderTexture(renderer, g_ressources->txtr_bgtile, NULL, &dest);
        }
    }

    SDL_SetRenderTarget(renderer, NULL);    //NULL is for window
}