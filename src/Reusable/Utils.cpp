#include "Utils.h"

#include <math.h>
#include <SDL_ttf.h>

#include "../Reusable/Globals.h"
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
