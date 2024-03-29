#include "FlipnoteFrame.h"

#include <math.h>
#include <SDL3/SDL.h>
#include <stdio.h>

#include "../../Reusable/RunState.h"
#include "FlipnoteLayer.h"
#include "Flipnote.h"


FlipnoteFrame::FlipnoteFrame(Flipnote* flipnote, int w, int h) {
    m_flipnote = flipnote;
    m_width = w;
    m_height = h;
    m_cachedtexture = NULL;

    if(m_width <= 0 || m_height <= 0) {
        printf("FlipnoteFrame::FlipnoteFrame : invalid size\n");
        return;
    }

    int layer_count = m_flipnote->GetLayerCount();
    for(int i = 0; i < layer_count; i++) {
        m_layers.push_back(new FlipnoteLayer(this, m_width, m_height));
    }

    m_cachedtexture = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, m_width, m_height);
    SDL_SetTextureBlendMode(m_cachedtexture, SDL_BLENDMODE_BLEND);
    UpdateCachedTexture();

}

FlipnoteFrame::FlipnoteFrame(Flipnote* flipnote, int w, int h, FILE* infile) {
    m_flipnote = flipnote;
    
    m_width = w;
    m_height = h;

    m_cachedtexture = NULL;

    if(m_width <= 0 || m_height <= 0) {
        printf("FlipnoteFrame::FlipnoteFrame : invalid size\n");
        return;
    }

    //Load layers from bottom to top
    int layer_count = m_flipnote->GetLayerCount();
    for(int i = 0; i < layer_count; i++) {
        m_layers.push_back(new FlipnoteLayer(this, m_width, m_height, infile));
    }

    m_cachedtexture = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, m_width, m_height);
    SDL_SetTextureBlendMode(m_cachedtexture, SDL_BLENDMODE_BLEND);
    UpdateCachedTexture();
}


FlipnoteFrame::~FlipnoteFrame() {
    for(FlipnoteLayer* l : m_layers) {
        delete l;
    }
    m_layers.clear();

    if(m_cachedtexture != NULL) {
        SDL_DestroyTexture(m_cachedtexture);
        m_cachedtexture = NULL;
    }
}


SDL_Texture* FlipnoteFrame::CopyToTexture(bool transparent) {
    //Create the texture
    SDL_Texture* r = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, m_width, m_height);
    SDL_SetTextureBlendMode(r, SDL_BLENDMODE_BLEND);    //the texture can have transparency
    printf("FlipnoteFrame::CopyToTexture : allocated texture\n");

    //Actually copy pixel data to the texture
    int return_code = OverwriteTexture(r, transparent);
    if(return_code != 0) {
        printf("FlipnoteFrame::CopyToTexture : OverwriteTexture did not return 0\n");
    }

    return r;
}


SDL_Texture* FlipnoteFrame::CopyToTexture(int w, int h, bool transparent) {
    SDL_Texture* previousrendertartget = SDL_GetRenderTarget(g_runstate->renderer);

    //TODO : these should be float ?
    int stepx = m_width/w;
    int stepy = m_height/h;

    //Create the texture
    SDL_Texture* r = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(r, SDL_BLENDMODE_BLEND);    //the texture can have transparency
    SDL_SetRenderTarget(g_runstate->renderer, r);
    SDL_SetRenderDrawBlendMode(g_runstate->renderer, SDL_BLENDMODE_NONE);   //treat alpha value of pixels the same way as other colors

    //Get the palette from the flipnote
    SDL_Color* pal = m_flipnote->GetPalette();

    //Clear with background color or transparency
    SDL_SetRenderDrawColor(g_runstate->renderer, pal[0].r, pal[0].g, pal[0].b, 255*!transparent);
    SDL_RenderClear(g_runstate->renderer);

    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            //Get the color index of the highest set pixel
            unsigned char p = GetPixel(x*stepx, y*stepy);

            //skip blank pixel
            if(p == 0) continue;

            //Draw the pixel on the texture with the color from the palette
            SDL_SetRenderDrawColor(g_runstate->renderer, pal[p].r, pal[p].g, pal[p].b, pal[p].a);
            SDL_RenderPoint(g_runstate->renderer, x, y);
        }
    }

    SDL_SetRenderTarget(g_runstate->renderer, previousrendertartget);
    return r;
}


std::vector<SDL_Texture*> FlipnoteFrame::CopyToTextures() {
    auto r = std::vector<SDL_Texture*>();

    //itterate from bottom to top
    int layer_count = m_layers.size();
    for(int i = 0; i < layer_count; i++) {
        r.push_back(m_layers[i]->CopyToTexture(true));
    }

    return r;
}



int FlipnoteFrame::OverwriteTexture(SDL_Texture* texture, bool transparent) {
    int w, h, access;
    SDL_QueryTexture(texture, NULL, &access, &w, &h);
    if(w != m_width || h != m_height || access != SDL_TEXTUREACCESS_TARGET) {
        return 1;   //not correct size or access
    }

    SDL_Texture* previousrendertarget = SDL_GetRenderTarget(g_runstate->renderer);
    SDL_SetRenderTarget(g_runstate->renderer, texture);

    //Clear with blank
    SDL_SetRenderDrawBlendMode(g_runstate->renderer, SDL_BLENDMODE_NONE);   //Because we are filling with blank
    SDL_SetRenderDrawColor(g_runstate->renderer, 255, 255, 0, 0);           //"yellow" for debugging purpose
    SDL_RenderClear(g_runstate->renderer);

    //Get the palette from the flipnote
    SDL_Color* pal = m_flipnote->GetPalette();

    ////Copy pixel data
    //for(int y = 0; y < m_height; y++) {
    //    for(int x = 0; x < m_width; x++) {
    //        //Get the color index of the highest set pixel
    //        unsigned char p = GetPixel(x, y);
    //
    //        if(p == 0 && transparent == true) continue;
    //
    //        //Draw the pixel on the texture with the color from the palette
    //        SDL_SetRenderDrawColor(g_runstate->renderer, pal[p].r, pal[p].g, pal[p].b, pal[p].a);
    //        SDL_RenderPoint(g_runstate->renderer, x, y);
    //    }
    //}

    //Draw all layers to the texture, from bottom to top
    int layer_count = m_layers.size();
    for(int i = 0; i < layer_count; i++) {
        SDL_Texture* layer_texture = m_layers[i]->CopyToTexture(transparent || i != 0);    //the bottom layer don't have a transparent background if transparent = false
        SDL_RenderTexture(g_runstate->renderer, layer_texture, NULL, NULL);
        SDL_DestroyTexture(layer_texture);
    }

    SDL_SetRenderTarget(g_runstate->renderer, previousrendertarget);
    return 0;
}


int FlipnoteFrame::OverwriteTextures(std::vector<SDL_Texture*>& textures) {
    int textures_count = textures.size();
    if(textures_count != m_flipnote->GetLayerCount() || textures_count == 0) {
        return 1;   //invalid count
    }

    int w, h;
    SDL_QueryTexture(textures[0], NULL, NULL, &w, &h);
    if(w != m_width || h != m_height) {
        return 2;   //not correct size
    }

    //itterate from bottom to top
    int layer_count = m_layers.size();
    for(int i = 0; i < layer_count; i++) {
        int return_code = m_layers[i]->OverwriteTexture(textures[i], true);
        if(return_code != 0) {
            printf("FlipnoteFrame::OverwriteTextures : OverwriteTexture did not return 0\n");
        }
    }

    return 0;   //success
}



void FlipnoteFrame::SetPixel(int x, int y, int layerindex, int colorindex) {
    if(layerindex < 0 || layerindex >= m_layers.size()) return;
    m_layers[layerindex]->SetPixel(x, y, colorindex);
}


unsigned char FlipnoteFrame::GetPixel(int x, int y) {
    if(x < 0 || x >= m_width || y < 0 || y >= m_height) 
        return 0;

    //itterate from top to bottom
    for(int i = m_layers.size()-1; i >= 0; i--) {
        int p = m_layers[i]->GetPixel(x, y);     //No need for check becasue we already do it above
        if(p != 0) return p;    //non-blank pixel
    }
    return 0;
}


void FlipnoteFrame::UpdateCachedTexture() {
    OverwriteTexture(m_cachedtexture, true);
    printf("FlipnoteFrame::UpdateCachedTexture : done\n");
}

SDL_Texture* FlipnoteFrame::GetCachedTexture() {
    return m_cachedtexture;
}

void FlipnoteFrame::Save(FILE* file) {
    //Save layers from bottom to top
    for(FlipnoteLayer* l : m_layers) {
        l->Save(file);
    }
}
