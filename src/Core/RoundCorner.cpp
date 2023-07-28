#include "RoundCorner.h"

#include <SDL3/SDL.h>

#include "../Reusable/RunState.h"
#include "../Reusable/Tileset.h"
#include "FlipnoteRessources.h"

void RoundTextureCorner(SDL_Texture* texture) {
    if(texture == NULL) return;

    int access;
    int w, h;
    SDL_QueryTexture(texture, NULL, &access, &w, &h);

    //Check if we can draw on the texture
    if(access != SDL_TEXTUREACCESS_TARGET) return;

    //Setup rounded corner texture correctly
    SDL_SetTextureColorMod(g_ressources->txtr_rounded_rect, 255, 255, 255); //white
    SDL_SetTextureBlendMode(g_ressources->txtr_rounded_rect, SDL_BLENDMODE_MOD);

    //Begin drawing on the texture
    SDL_Texture* oldrendertarget = SDL_GetRenderTarget(g_runstate->renderer);
    SDL_SetRenderTarget(g_runstate->renderer, texture);

    //Add the rounded corners
    g_ressources->tileset_rounded_rect->DrawRectangle(g_runstate->renderer, 0, 0, w, h, g_ressources->rectdata_basic_rect);

    //Restore rounded corner
    SDL_SetRenderTarget(g_runstate->renderer, oldrendertarget);

    //Restore the rounded corner's original blend mode
    SDL_SetTextureBlendMode(g_ressources->txtr_rounded_rect, SDL_BLENDMODE_BLEND);
}
