#include "FlipnoteLayer.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>

#include "../../Reusable/RunState.h"
#include "FlipnoteFrame.h"
#include "Flipnote.h"

FlipnoteLayer::FlipnoteLayer(FlipnoteFrame* frame, int w, int h) {
    m_frame = frame;
    
    m_width = w;
    m_height = h;
    if(w < 0 || h < 0) {
        m_pixels = nullptr;
        return;
    }

    //Allocate pixels and fill with first color
    m_pixels = (unsigned char*) malloc(sizeof(unsigned char) * w * h);
    int pixelcount = w*h;
    for(int i = 0; i<pixelcount; i++) m_pixels[i] = 0;
}

FlipnoteLayer::FlipnoteLayer(FlipnoteFrame* frame, int w, int h, FILE* infile) {
    m_frame = frame;

    m_width = w;
    m_height = h;
    if(w < 0 || h < 0) {
        m_pixels = nullptr;
        return;
    }

    //Allocate and load pixels
    m_pixels = (unsigned char*) malloc(sizeof(unsigned char) * w * h);
    int pixelcount = w*h;
    for(int i = 0; i<pixelcount; i++) m_pixels[i] = getc(infile);
}

FlipnoteLayer::~FlipnoteLayer() {
    if(m_pixels != nullptr) {
        free(m_pixels);
        m_pixels = nullptr;
    }
}


SDL_Texture* FlipnoteLayer::CopyToTexture(bool transparentbackground) {

    //Create the texture
    SDL_Texture* r = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, m_width, m_height);
    SDL_SetTextureBlendMode(r, SDL_BLENDMODE_BLEND);    //Textures can have transparency
    printf("FlipnoteLayer::CopyToTexture : allocated texture\n");

    //Actually copy the data to the texture
    int return_code = OverwriteTexture(r, transparentbackground);
    if(return_code != 0) {
        printf("FlipnoteLayer::CopyToTexture : OverwriteTexture did not return 0\n");
    }
    
    return r;
}


SDL_Texture* FlipnoteLayer::CopyToTexture(int w, int h, bool transparentbackground) {
    SDL_Texture* previousrendertarget = SDL_GetRenderTarget(g_runstate->renderer);

    int stepx = m_width/w;
    int stepy = m_height/h;

    //Create the texture
    SDL_Texture* r = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(g_runstate->renderer, r);
    SDL_SetTextureBlendMode(r, SDL_BLENDMODE_BLEND);
    printf("FlipnoteLayer::CopyToTexture : allocated texture\n");

    //TODO : Check if this is working and/or necessary
    SDL_SetRenderDrawBlendMode(g_runstate->renderer, SDL_BLENDMODE_NONE);
    

    //Get the palette from the flipnote
    SDL_Color* pal = m_frame->m_flipnote->GetPalette();


    //Clear with transparency OR the first color of the palette
    if(transparentbackground) SDL_SetRenderDrawColor(g_runstate->renderer, 255, 0, 0, 0);       //"red" for debugging purpose
    else SDL_SetRenderDrawColor(g_runstate->renderer, pal[0].r, pal[0].g, pal[0].b, pal[0].a);
    SDL_RenderClear(g_runstate->renderer);


    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            //Get the palette index of the pixel
            unsigned char p = GetPixel(x*stepx, y*stepy);

            //If the color is 0 (bg color for bottom layer or blank for other layers) don't draw it
            if(p == 0) continue;

            //Draw the pixel to the texture
            SDL_SetRenderDrawColor(g_runstate->renderer, pal[p].r, pal[p].g, pal[p].b, pal[p].a);
            SDL_RenderPoint(g_runstate->renderer, x, y);
        }
    }

    SDL_SetRenderTarget(g_runstate->renderer, previousrendertarget);
    return r;
}

int FlipnoteLayer::OverwriteTexture(SDL_Texture* texture, bool transparentbackground) {
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    if(w != m_width || h != m_height) {
        return 1;   //invalid size
    }


    SDL_Texture* previousrendertarget = SDL_GetRenderTarget(g_runstate->renderer);
    SDL_SetRenderTarget(g_runstate->renderer, texture);
    //TODO : Check if this is working and/or necessary
    SDL_SetRenderDrawBlendMode(g_runstate->renderer, SDL_BLENDMODE_NONE);


    //Get the palette from the flipnote
    SDL_Color* pal = m_frame->m_flipnote->GetPalette();


    //Clear with transparency OR the first color of the palette
    if(transparentbackground) SDL_SetRenderDrawColor(g_runstate->renderer, 255, 0, 0, 0);       //"red" for debugging purpose
    else SDL_SetRenderDrawColor(g_runstate->renderer, pal[0].r, pal[0].g, pal[0].b, pal[0].a);
    SDL_RenderClear(g_runstate->renderer);

    int i = 0;
    for(int y = 0; y < m_height; y++) {
        for(int x = 0; x < m_width; x++) {
            //Get the palette index of the pixel
            unsigned char p = m_pixels[i];    //m_pixels[x+y*m_width];

            //If the color is 0 (bg color for bottom layer or blank for other layers) don't draw it
            if(p == 0) {i++; continue;}

            //Draw the pixel to the texture
            SDL_SetRenderDrawColor(g_runstate->renderer, pal[p].r, pal[p].g, pal[p].b, pal[p].a);
            SDL_RenderPoint(g_runstate->renderer, x, y);
            i++;
        }
    }

    SDL_SetRenderTarget(g_runstate->renderer, previousrendertarget);
    return 0;
}


void FlipnoteLayer::SetPixel(int x, int y, int colorindex) {
    if(x < 0 || x >= m_width || y < 0 || y >= m_height) 
        return;

    m_pixels[x+y*m_width] = colorindex;
}


unsigned char FlipnoteLayer::GetPixel(int x, int y) {
    if(x < 0 || x >= m_width || y < 0 || y >= m_height) 
        return 0;

    return m_pixels[x+y*m_width];
}


void FlipnoteLayer::Save(FILE* file) {
    int istop = m_width*m_height;
    for(int i = 0; i < istop; i++) {
        putc(m_pixels[i], file);
    }
}
