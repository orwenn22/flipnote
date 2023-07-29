#include "FlipnoteRessources.h"

#include <SDL3/SDL_ttf.h>
#include <SDL3/SDL_image.h>

#include <stdio.h>

#include "../Reusable/Tileset.h"


FlipnoteRessources* g_ressources = nullptr;


FlipnoteRessources::FlipnoteRessources(SDL_Renderer* renderer)
: Ressources(renderer) {
    txtr_bgtile = LoadImageAsTexture(renderer, "./res/GridBackgroundTile_2x.png");
    txtr_button_pen = LoadImageAsTexture(renderer, "./res/pen.png");
    txtr_button_smallpen = LoadImageAsTexture(renderer, "./res/smallpen.png");
    txtr_timelinetile = LoadImageAsTexture(renderer, "./res/FrameBgTile.png");
    txtr_button_timeline = LoadImageAsTexture(renderer, "./res/timelineicon.png");
    txtr_icon_add = LoadImageAsTexture(renderer, "./res/fluent_add.png");
    txtr_icon_delete = LoadImageAsTexture(renderer, "./res/fluent_delete.png");
    txtr_icon_right = LoadImageAsTexture(renderer, "./res/fluent_right.png");
    txtr_icon_left = LoadImageAsTexture(renderer, "./res/fluent_left.png");
    txtr_icon_right_double = LoadImageAsTexture(renderer, "./res/fluent_right_double.png");
    txtr_icon_left_double = LoadImageAsTexture(renderer, "./res/fluent_left_double.png");
    txtr_button_play = LoadImageAsTexture(renderer, "./res/button_play.png");
    txtr_button_pause = LoadImageAsTexture(renderer, "./res/button_pause.png");
    txtr_button_playback = LoadImageAsTexture(renderer, "./res/button_playback.png");
    txtr_button_playback_begginning = LoadImageAsTexture(renderer, "./res/button_playback_beginning.png");
    txtr_button_playforward = LoadImageAsTexture(renderer, "./res/button_playforward.png");
    txtr_button_playforward_end = LoadImageAsTexture(renderer, "./res/button_playforward_end.png");
    txtr_button_frog = LoadImageAsTexture(renderer, "./res/frog_menu_plain.png");
    txtr_button_layer_active = LoadImageAsTexture(renderer, "./res/button_layer_active.png");
    txtr_button_layer_inactive = LoadImageAsTexture(renderer, "./res/button_layer_inactive.png");
    txtr_icon_red_arrow = LoadImageAsTexture(renderer, "./res/redarrow.png");
    txtr_icon_yellow_arrow = LoadImageAsTexture(renderer, "./res/yellowarrow.png");

    txtr_rounded_rect = LoadImageAsTexture(renderer, "./res/rounded_rect.png");
    tileset_rounded_rect = new Tileset(txtr_rounded_rect, 16, 16);

    txtr_rounded_rect_line = LoadImageAsTexture(renderer, "./res/rounded_rect_line.png");
    tileset_rounded_rect_line = new Tileset(txtr_rounded_rect_line, 16, 16);

    rectdata_basic_rect = new TilesetRectData({0, 1, 2, 3, 4, 5, 6, 7, 8});

    //destRGB = srcRGB*0 + destRGB*1
    //destA   = srcA*1   + destA*0
    blendmode_alphamask = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);
}

FlipnoteRessources::~FlipnoteRessources() {
    SDL_DestroyTexture(txtr_bgtile);
    SDL_DestroyTexture(txtr_button_pen);
    SDL_DestroyTexture(txtr_button_smallpen);
    SDL_DestroyTexture(txtr_timelinetile);
    SDL_DestroyTexture(txtr_button_timeline);
    SDL_DestroyTexture(txtr_icon_add);
    SDL_DestroyTexture(txtr_icon_delete);
    SDL_DestroyTexture(txtr_icon_right);
    SDL_DestroyTexture(txtr_icon_left);
    SDL_DestroyTexture(txtr_icon_right_double);
    SDL_DestroyTexture(txtr_icon_left_double);
    SDL_DestroyTexture(txtr_button_play);
    SDL_DestroyTexture(txtr_button_pause);
    SDL_DestroyTexture(txtr_button_playback);
    SDL_DestroyTexture(txtr_button_playback_begginning);
    SDL_DestroyTexture(txtr_button_playforward);
    SDL_DestroyTexture(txtr_button_playforward_end);
    SDL_DestroyTexture(txtr_button_frog);
    SDL_DestroyTexture(txtr_button_layer_active);
    SDL_DestroyTexture(txtr_button_layer_inactive);
    SDL_DestroyTexture(txtr_icon_red_arrow);
    SDL_DestroyTexture(txtr_icon_yellow_arrow);

    delete tileset_rounded_rect;
    SDL_DestroyTexture(txtr_rounded_rect);

    delete tileset_rounded_rect_line;
    SDL_DestroyTexture(txtr_rounded_rect_line);

    delete rectdata_basic_rect;

    printf("FlipnoteRessources::~FlipnoteRessources : unloaded everything\n");
}
