#include "FlipnoteFrame.h"

#include <math.h>
#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../Reusable/RunState.h"
#include "../../Reusable/Utils.h"
#include "../Globals.h"

//FIXME : 8 colors hardcoded
static SDL_Color defaultframepalette[] = {
    {255, 255, 255, 255},
    {  0,   0,   0, 255},
    {200,   0,   0, 255},
    {  0,   0, 200, 255},
    {0  , 200,   0, 255},
    {170, 170, 170, 255},
    {200,   0, 200, 255},
    {200, 200,   0, 255},
};
static const int defaultframepalettesize = sizeof(defaultframepalette) / sizeof(defaultframepalette[0]);

FlipnoteFrame::FlipnoteFrame(int w, int h) {
    m_width = w;
    m_height = h;
    if(w < 0 || h < 0) {
        m_pixels = NULL;
        return;
    }

    m_pixels = (unsigned char*) malloc(sizeof(unsigned char) * w * h);

    //FIXME : 8 colors hardcoded
    m_colors = (SDL_Color*) malloc(sizeof(SDL_Color) * 8);
    m_colors[0] = {255, 255, 255, 255};
    m_colors[1] = {  0,   0,   0, 255};
    m_colors[2] = {200,   0,   0, 255};
    m_colors[3] = {  0,   0, 200, 255};
    m_colors[4] = {0  , 200,   0, 255};
    m_colors[5] = {170, 170, 170, 255};
    m_colors[6] = {200,   0, 200, 255};
    m_colors[7] = {200, 200,   0, 255};

    int pixelcount = w*h;
    for(int i = 0; i<pixelcount; i++) m_pixels[i] = 0;
}

FlipnoteFrame::FlipnoteFrame(int w, int h, FILE* infile) {
    m_width = w;
    m_height = h;
    if(w < 0 || h < 0) {
        m_pixels = NULL;
        return;
    }

    //FIXME : 8 colors hardcoded
    m_colors = (SDL_Color*) malloc(sizeof(SDL_Color) * 8);
    printf("FlipnoteFrame::FlipnoteFrame : loading palette from file : ");
    for(SDL_Color* colptr = m_colors; colptr < m_colors + 8;colptr += 1) {
        colptr->r = getc(infile);
        colptr->g = getc(infile);
        colptr->b = getc(infile);
        colptr->a = getc(infile);
        printf("\x1b[48;2;%u;%u;%um  \x1b[0m", colptr->r, colptr->g, colptr->b);    //ANSI escape code for changing terminal's background color
    }
    printf("\n");


    m_pixels = (unsigned char*) malloc(sizeof(unsigned char) * w * h);
    int pixelcount = w*h;
    for(int i = 0; i<pixelcount; i++) m_pixels[i] = getc(infile);
}


FlipnoteFrame::~FlipnoteFrame() {
    free(m_pixels);
    free(m_colors);
}


SDL_Texture* FlipnoteFrame::CopyToTexture() {
    SDL_Texture* previousrendertartget = SDL_GetRenderTarget(g_runstate->renderer);

    SDL_Texture* r = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, m_width, m_height);
    SDL_SetRenderTarget(g_runstate->renderer, r);

    SDL_SetRenderDrawColor(g_runstate->renderer, 255, 255, 255, 255);
    SDL_RenderClear(g_runstate->renderer);

    int i = 0;
    for(int y = 0; y < m_height; y++) {
        for(int x = 0; x < m_width; x++) {
            unsigned char p = m_pixels[i];    //m_pixels[x+y*m_width];
            SDL_SetRenderDrawColor(g_runstate->renderer, m_colors[p].r, m_colors[p].g, m_colors[p].b, m_colors[p].a);
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

    SDL_Texture* r = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(g_runstate->renderer, r);

    SDL_SetRenderDrawColor(g_runstate->renderer, 255, 255, 255, 255);
    SDL_RenderClear(g_runstate->renderer);

    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            unsigned char p = GetPixel(x*stepx, y*stepy);
            SDL_SetRenderDrawColor(g_runstate->renderer, m_colors[p].r, m_colors[p].g, m_colors[p].b, m_colors[p].a);
            SDL_RenderPoint(g_runstate->renderer, x, y);
        }
    }

    SDL_SetRenderTarget(g_runstate->renderer, previousrendertartget);
    return r;
}


void FlipnoteFrame::SetColor(int index, SDL_Color c) {
    //FIXME : 8 colors hardcoded
    if(index < 0) index = 0;
    if(index > 7) index = 7;
    m_colors[index] = c;
}

SDL_Color FlipnoteFrame::GetColor(int index) {
    //FIXME : 8 colors hardcoded
    if(index < 0) index = 0;
    if(index > 7) index = 7;
    return m_colors[index];
}

SDL_Color* FlipnoteFrame::GetPalette() {
    return m_colors;
}


//static
SDL_Color FlipnoteFrame::GetDefaultColor(int index) {
    if(index < 0) index = 0;
    if(index > defaultframepalettesize) index = defaultframepalettesize-1;
    return defaultframepalette[index];
}

SDL_Color* FlipnoteFrame::GetDefaultPalette() {
    return defaultframepalette;
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
    //FIXME : 8 color colors
    printf("FlipnoteFrame::Save : saving palette to file : ");
    for(SDL_Color* colptr = m_colors; colptr < m_colors + 8;colptr += 1) {
        putc(colptr->r, file);
        putc(colptr->g, file);
        putc(colptr->b, file);
        putc(colptr->a, file);
        printf("\x1b[48;2;%u;%u;%um  \x1b[0m", colptr->r, colptr->g, colptr->b);    //ANSI escape code for changing terminal's background color
    }
    printf("\n");

    int istop = m_width*m_height;
    for(int i = 0; i < istop; i++) {
        putc(m_pixels[i], file);
    }
}
