#ifndef REUSABLE_RESSOURCES_H
#define REUSABLE_RESSOURCES_H

class Tileset;

struct _TTF_Font;
struct SDL_Color;
struct SDL_Renderer;
struct SDL_Texture;
struct TilesetRectData;

SDL_Texture* LoadImageAsTexture(SDL_Renderer* renderer, const char *file);

struct Ressources {
    _TTF_Font* font_ubuntumedium24;
    _TTF_Font* font_ubuntumedium16;

    SDL_Texture* txtr_cross;
    SDL_Texture* txtr_maximize;

    SDL_Texture* txtr_popupmenutiles;
    SDL_Texture* txtr_checkboxfalse;
    SDL_Texture* txtr_checkboxtrue;

    Tileset* tileset_popupmenu;
    TilesetRectData* rectdata_popupmenu;    //for Tileset->DrawRectangle

    SDL_Color* col_orange;
    SDL_Color* col_white;

    Ressources(SDL_Renderer* renderer);
    virtual ~Ressources();
};

extern Ressources* g_reusableressources;

#endif
