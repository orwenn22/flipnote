#include "Ressources.h"

#include <SDL_ttf.h>
#include <SDL_image.h>

#include <stdio.h>

#include "Tileset.h"


SDL_Texture* LoadImageAsTexture(SDL_Renderer* renderer, const char *file) {
    SDL_Surface* tmp = IMG_Load(file);
    if(!tmp) {
        printf("error loading %s\n", file);
        return NULL;
    }

    SDL_Texture* r = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_DestroySurface(tmp);

    if(!r) {
        printf("error converting %s to texture\n", file);
        return NULL;
    }

    return r;
}

Ressources::Ressources(SDL_Renderer* renderer) {
    font_ubuntumedium24 = TTF_OpenFont("./res/Ubuntu-Medium.ttf", 24);
    font_ubuntumedium16 = TTF_OpenFont("./res/Ubuntu-Medium.ttf", 16);

    txtr_cross = LoadImageAsTexture(renderer, "./res/close.png");
    txtr_bgtile = LoadImageAsTexture(renderer, "./res/GridBackgroundTile_2x.png");
    txtr_maximize = LoadImageAsTexture(renderer, "./res/maximize.png");
    txtr_penbutton = LoadImageAsTexture(renderer, "./res/pen.png");
    txtr_smallpenbutton = LoadImageAsTexture(renderer, "./res/smallpen.png");
    txtr_popupmenutiles = LoadImageAsTexture(renderer, "./res/popupmenutiles.png");
    txtr_checkboxfalse = LoadImageAsTexture(renderer, "./res/checkbox_empty.png");
    txtr_checkboxtrue = LoadImageAsTexture(renderer, "./res/checkbox_2.png");

    tileset_popupmenu = new Tileset(txtr_popupmenutiles, 16, 16);
    rectdata_popupmenu = new TilesetRectData({21, 22, 23, 27, 28, 29, 33, 34, 35});

    col_orange = new SDL_Color({0xfb, 0x61, 0x01, 0xff});
    col_white = new SDL_Color({0xff, 0xff, 0xff, 0xff});
}

Ressources::~Ressources() {
    TTF_CloseFont(font_ubuntumedium24);
    TTF_CloseFont(font_ubuntumedium16);

    SDL_DestroyTexture(txtr_cross);
    SDL_DestroyTexture(txtr_bgtile);
    SDL_DestroyTexture(txtr_maximize);
    SDL_DestroyTexture(txtr_penbutton);
    SDL_DestroyTexture(txtr_smallpenbutton);
    SDL_DestroyTexture(txtr_popupmenutiles);
    SDL_DestroyTexture(txtr_checkboxfalse);
    SDL_DestroyTexture(txtr_checkboxtrue);

    delete rectdata_popupmenu;
    delete tileset_popupmenu;
    
    delete col_orange;
    delete col_white;
}