#include "DSThumbnail.h"

#include <SDL.h>
#include <stdio.h>

#include "../Reusable/RunState.h"
#include "../Globals.h"

SDL_Color thumbnailcolors[16] = {
    {0xff, 0xff, 0xff, 0xff},   //0
    {0x52, 0x52, 0x52, 0xff},   //1
    {0xff, 0xff, 0xff, 0xff},   //2
    {0x9c, 0x9c, 0x9c, 0xff},   //3
    {0xff, 0x48, 0x44, 0xff},   //4
    {0xc8, 0x51, 0x4f, 0xff},   //5
    {0xff, 0xad, 0xac, 0xff},   //6
    {0x00, 0xff, 0x00, 0xff},   //7
    {0x48, 0x40, 0xff, 0xff},   //8
    {0x51, 0x4f, 0xb8, 0xff},   //9
    {0xad, 0xab, 0xff, 0xff},   //10
    {0x00, 0xff, 0x00, 0xff},   //11
    {0xb6, 0x57, 0xb7, 0xff},   //12
    {0x00, 0xff, 0x00, 0xff},   //13
    {0x00, 0xff, 0x00, 0xff},   //14
    {0x00, 0xff, 0x00, 0xff},   //15
};

DSThumbnail::DSThumbnail(SDL_Renderer* renderer, const char* filename) {
    m_thumbnailtexture = NULL;

    FILE* infile = fopen(filename, "r");
    if(infile == NULL) return;

    if(!(getc(infile) == 'P' && getc(infile) == 'A' && getc(infile) == 'R' && getc(infile) == 'A')) {
        fclose(infile);
        return;
    }

    fseek(infile, 0xA0, SEEK_SET);
    unsigned char data[1536] = { 0 };
    for(int i = 0; i < 1536; i++) {
        data[i] = getc(infile);
    }
    int data_offset = 0;

    m_thumbnailtexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 64, 48);
    SDL_SetRenderTarget(renderer, m_thumbnailtexture);

    for(int tile_y = 0; tile_y < 48; tile_y+=8) {
        for(int tile_x = 0; tile_x < 64; tile_x += 8) {
            for(int line = 0; line < 8; line += 1) {
                for(int pixel = 0; pixel < 8; pixel += 2) {
                    int x = tile_x + pixel;
                    int y = tile_y + line;

                    SDL_Color* c = &(thumbnailcolors[data[data_offset] & 0x0F]);
                    SDL_SetRenderDrawColor(renderer, c->r, c->g, c->b, c->a);
                    SDL_RenderPoint(renderer, x, y);

                    c = &(thumbnailcolors[(data[data_offset] >> 4) & 0x0F]);
                    SDL_SetRenderDrawColor(renderer, c->r, c->g, c->b, c->a);
                    SDL_RenderPoint(renderer, x+1, y);
                    data_offset++;
                }
            }
        }
    }

    SDL_SetRenderTarget(renderer, NULL);    //null for window

    fclose(infile);
}

DSThumbnail::~DSThumbnail() {
    if(m_thumbnailtexture != NULL) {
        SDL_DestroyTexture(m_thumbnailtexture);
        m_thumbnailtexture = NULL;
    }
}

void DSThumbnail::Render(SDL_Renderer* renderer, float x, float y, float scale) {
    if(m_thumbnailtexture == NULL) return;
    
    SDL_FRect dest = {x, y, 64*scale, 48*scale};
    SDL_RenderTexture(renderer, m_thumbnailtexture, NULL, &dest);
}