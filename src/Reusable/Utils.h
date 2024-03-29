#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <SDL3/SDL.h>

struct _TTF_Font;

Uint32 SDLColorToUint32(SDL_Color c);

//Not optimised, should not be overused.
//Intended for debug purpose only.
void RenderText(const char* str, int x, int y, _TTF_Font* font, SDL_Color fg);                  //transparent bg
void RenderText(const char* str, int x, int y, _TTF_Font* font, SDL_Color fg, SDL_Color bg);    //solid bg

SDL_Texture* MakeTextTexture(const char* str, _TTF_Font* font, SDL_Color fg);                   //transparent bg

//Hittest that can be used with topbar
SDL_HitTestResult SDLCALL BorderlessHitTest(SDL_Window *win, const SDL_Point *area, void *data);

std::string GetCWD();

//Will put a NULL at the last byte of buf
void freadbytes(char* buf, size_t byteccount, FILE* infine);

std::string RemovePath(std::string s);
std::string RemoveExtension(std::string s);

#endif
