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


    /////////////////////////////////////
    //Colors

    SDL_Color* col_orange;
    SDL_Color* col_green;
    SDL_Color* col_white;

    //////////////////////////////////////
    //Rounded rectangle related stuff

    SDL_Texture* txtr_rounded_rect;
    Tileset* tileset_rounded_rect;

    SDL_Texture* txtr_rounded_rect_line;
    Tileset* tileset_rounded_rect_line;

    TilesetRectData* rectdata_basic_rect;


    ///////////////////////////////////////

    //This blend mode is used as an "alpha mask", it will apply the src's alpha on the dest's alpha
    //destRGB = srcRGB*0 + destRGB*1
    //destA   = srcA*1   + destA*0
    unsigned int blendmode_alphamask;

    Ressources(SDL_Renderer* renderer);
    virtual ~Ressources();
};

//Used by every gui stuff in the Reusable folder. Must be initalised in main.
//ex : g_reusableressources = new Ressources(...);
// -- or --
//ex : g_customressources = new CustomRessources(...);
//     g_reusableressources = g_customressources;
extern Ressources* g_reusableressources;

#endif
