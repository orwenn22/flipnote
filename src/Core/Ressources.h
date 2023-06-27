#ifndef RESSOURCES_H
#define RESSOURCES_H


class Tileset;

struct _TTF_Font;
struct SDL_Color;
struct SDL_Renderer;
struct SDL_Texture;
struct TilesetRectData;

struct Ressources {
    _TTF_Font* font_ubuntumedium24;
    _TTF_Font* font_ubuntumedium16;

    SDL_Texture* txtr_cross;
    SDL_Texture* txtr_maximize;
    SDL_Texture* txtr_bgtile;
    SDL_Texture* txtr_popupmenutiles;
    SDL_Texture* txtr_checkboxfalse;
    SDL_Texture* txtr_checkboxtrue;
    SDL_Texture* txtr_timelinetile;

    SDL_Texture* txtr_penbutton;
    SDL_Texture* txtr_smallpenbutton;
    SDL_Texture* txtr_timelineicon;
    SDL_Texture* txtr_playbutton;
    SDL_Texture* txtr_pausebutton;


    SDL_Texture* txtr_icon_add;
    SDL_Texture* txtr_icon_delete;
    SDL_Texture* txtr_icon_right;
    SDL_Texture* txtr_icon_left;
    SDL_Texture* txtr_icon_right_double;
    SDL_Texture* txtr_icon_left_double;


    Tileset* tileset_popupmenu;
    TilesetRectData* rectdata_popupmenu;    //for Tileset->DrawRectangle

    SDL_Color* col_orange;
    SDL_Color* col_white;
    
    Ressources(SDL_Renderer* renderer);
    ~Ressources();
};

#endif