#include "FlipnoteRessources.h"

#include <SDL_ttf.h>
#include <SDL_image.h>

#include <stdio.h>


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
}
