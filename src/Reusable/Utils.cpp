#include "Utils.h"

#include <math.h>
#include <SDL_ttf.h>

#include "../Reusable/RunState.h"


Uint32 SDLColorToUint32(SDL_Color c) {
    return ((c.a << 24) | (c.r << 16) | (c.g << 8) | c.b);
}


void RenderText(const char* str, int x, int y, _TTF_Font* font, SDL_Color fg) {
	if(font == nullptr) return;

	SDL_Surface* s = TTF_RenderText_Blended(font, str, fg);
	SDL_Texture* t = SDL_CreateTextureFromSurface(g_runstate->renderer, s);
	SDL_DestroySurface(s);

	int w, h;
	SDL_QueryTexture(t, NULL, NULL, &w, &h);


	SDL_FRect dest = {(float)x, (float)y, (float)w, (float)h};
	SDL_RenderTexture(g_runstate->renderer, t, NULL, &dest);

	SDL_DestroyTexture(t);
}


void RenderText(const char* str, int x, int y, _TTF_Font* font, SDL_Color fg, SDL_Color bg) {
	if(font == nullptr) return;
	
	SDL_Surface* s = TTF_RenderText_Shaded(font, str, fg, bg);
	SDL_Texture* t = SDL_CreateTextureFromSurface(g_runstate->renderer, s);
	SDL_DestroySurface(s);

	int w, h;
	SDL_QueryTexture(t, NULL, NULL, &w, &h);


	SDL_FRect dest = {(float)x, (float)y, (float)w, (float)h};
	SDL_RenderTexture(g_runstate->renderer, t, NULL, &dest);

	SDL_DestroyTexture(t);
}


SDL_Texture* MakeTextTexture(const char* str, _TTF_Font* font, SDL_Color fg) {
	if(font == nullptr) return nullptr;

	SDL_Surface* s = TTF_RenderText_Blended(font, str, fg);
	SDL_Texture* t = SDL_CreateTextureFromSurface(g_runstate->renderer, s);
	SDL_DestroySurface(s);
	return t;
}


SDL_HitTestResult SDLCALL
BorderlessHitTest(SDL_Window *win, const SDL_Point *area, void *data) {
	int ww;
    int wh;
    SDL_GetWindowSize(win, &ww, &wh);

    if((SDL_GetWindowFlags(g_runstate->window) & SDL_WINDOW_MAXIMIZED) == 0) {      //not maximized
        if(area->y < 7 && area->x < 7)              return SDL_HITTEST_RESIZE_TOPLEFT;
        else if(area->y < 7 && area->x >= ww-7)     return SDL_HITTEST_RESIZE_TOPRIGHT;
        else if(area->y >= wh-7 && area->x < 7)     return SDL_HITTEST_RESIZE_BOTTOMLEFT;
        else if(area->y >= wh-7 && area->x >= ww-7) return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
        else if(area->y < 7)                        return SDL_HITTEST_RESIZE_TOP;
        else if(area->x < 7)                        return SDL_HITTEST_RESIZE_LEFT;
        else if(area->y >= wh-7)                    return SDL_HITTEST_RESIZE_BOTTOM;
        else if(area->x >= ww-7)                    return SDL_HITTEST_RESIZE_RIGHT;
    }
    if(area->y < 30 && area->x < ww-60) {
        return SDL_HITTEST_DRAGGABLE;
    }
    return SDL_HITTEST_NORMAL;
}
