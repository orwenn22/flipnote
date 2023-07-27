#ifndef FLIPNOTE_RESSOURCES_H
#define FLIPNOTE_RESSOURCES_H

#include "../Reusable/Ressources.h"

struct FlipnoteRessources : Ressources {
    SDL_Texture* txtr_bgtile;
    SDL_Texture* txtr_timelinetile;

    SDL_Texture* txtr_button_pen;
    SDL_Texture* txtr_button_smallpen;
    SDL_Texture* txtr_button_timeline;
    SDL_Texture* txtr_button_play;
    SDL_Texture* txtr_button_pause;
    SDL_Texture* txtr_button_playback;
    SDL_Texture* txtr_button_playback_begginning;
    SDL_Texture* txtr_button_playforward;
    SDL_Texture* txtr_button_playforward_end;
    SDL_Texture* txtr_button_frog;
    SDL_Texture* txtr_button_layer_active;
    SDL_Texture* txtr_button_layer_inactive;

    SDL_Texture* txtr_icon_add;
    SDL_Texture* txtr_icon_delete;
    SDL_Texture* txtr_icon_right;
    SDL_Texture* txtr_icon_left;
    SDL_Texture* txtr_icon_right_double;
    SDL_Texture* txtr_icon_left_double;
    SDL_Texture* txtr_icon_red_arrow;
    SDL_Texture* txtr_icon_yellow_arrow;

    SDL_Texture* txtr_rounded_rect;
    Tileset* tileset_rounded_rect;

    SDL_Texture* txtr_rounded_rect_line;
    Tileset* tileset_rounded_rect_line;

    TilesetRectData* rectdata_basic_rect;


    
    FlipnoteRessources(SDL_Renderer* renderer);
    virtual ~FlipnoteRessources();
};

//Used by every graphical stuff not in the Reusable folder.
//Initialised in main
extern FlipnoteRessources* g_ressources;

#endif
