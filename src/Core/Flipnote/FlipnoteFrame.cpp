#include "FlipnoteFrame.h"

#include <math.h>
#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../Reusable/RunState.h"
#include "../../Reusable/Utils.h"
#include "Flipnote.h"


FlipnoteFrame::FlipnoteFrame(Flipnote* flipnote, int w, int h) {
    m_flipnote = flipnote;
    m_width = w;
    m_height = h;
    if(w < 0 || h < 0) {
        m_pixels = NULL;
        return;
    }

    //Allocate pixels and fill with first color
    m_pixels = (unsigned char*) malloc(sizeof(unsigned char) * w * h);
    int pixelcount = w*h;
    for(int i = 0; i<pixelcount; i++) m_pixels[i] = 0;
}

FlipnoteFrame::FlipnoteFrame(Flipnote* flipnote, int w, int h, FILE* infile) {
    m_flipnote = flipnote;
    m_width = w;
    m_height = h;
    if(w < 0 || h < 0) {
        m_pixels = NULL;
        return;
    }

    //Allocate and load pixels
    m_pixels = (unsigned char*) malloc(sizeof(unsigned char) * w * h);
    int pixelcount = w*h;
    for(int i = 0; i<pixelcount; i++) m_pixels[i] = getc(infile);
}


FlipnoteFrame::~FlipnoteFrame() {
    free(m_pixels);
}


SDL_Texture* FlipnoteFrame::CopyToTexture() {
    SDL_Texture* previousrendertartget = SDL_GetRenderTarget(g_runstate->renderer);

    SDL_Texture* r = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_TARGET, m_width, m_height);
    SDL_SetRenderTarget(g_runstate->renderer, r);

    SDL_SetRenderDrawColor(g_runstate->renderer, 255, 255, 255, 255);
    SDL_RenderClear(g_runstate->renderer);

    SDL_Color* pal = m_flipnote->GetPalette();
    int i = 0;
    for(int y = 0; y < m_height; y++) {
        for(int x = 0; x < m_width; x++) {
            unsigned char p = m_pixels[i];    //m_pixels[x+y*m_width];
            SDL_SetRenderDrawColor(g_runstate->renderer, pal[p].r, pal[p].g, pal[p].b, pal[p].a);
            SDL_RenderPoint(g_runstate->renderer, x, y);
            i++;
        }
    }

    SDL_SetRenderTarget(g_runstate->renderer, previousrendertartget);
    return r;
}


SDL_Texture* FlipnoteFrame::CopyToTexture(int w, int h) {
    SDL_Texture* previousrendertartget = SDL_GetRenderTarget(g_runstate->renderer);

    int stepx = m_width/w;
    int stepy = m_height/h;

    SDL_Texture* r = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(g_runstate->renderer, r);

    SDL_SetRenderDrawColor(g_runstate->renderer, 255, 255, 255, 255);
    SDL_RenderClear(g_runstate->renderer);

    SDL_Color* pal = m_flipnote->GetPalette();
    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            unsigned char p = GetPixel(x*stepx, y*stepy);
            SDL_SetRenderDrawColor(g_runstate->renderer, pal[p].r, pal[p].g, pal[p].b, pal[p].a);
            SDL_RenderPoint(g_runstate->renderer, x, y);
        }
    }

    SDL_SetRenderTarget(g_runstate->renderer, previousrendertartget);
    return r;
}



void FlipnoteFrame::SetPixel(int x, int y, int colorindex) {
    if(x < 0 || x >= m_width || y < 0 || y >= m_height) 
        return;

    m_pixels[x+y*m_width] = colorindex;
}


unsigned char FlipnoteFrame::GetPixel(int x, int y) {
    if(x < 0 || x >= m_width || y < 0 || y >= m_height) 
        return 0;

    return m_pixels[x+y*m_width];
}


void FlipnoteFrame::Save(FILE* file) {
    int istop = m_width*m_height;
    for(int i = 0; i < istop; i++) {
        putc(m_pixels[i], file);
    }
}
