#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>

Uint32 SDLColorToUint32(SDL_Color c);

void SurfaceSetPixel(SDL_Surface* surface, int x, int y, Uint32 col);

void SurfaceDrawLine(SDL_Surface *surface, int x0, int y0, int x1, int y1, Uint32 col);
void SurfaceDrawLineEx(SDL_Surface *surface, int x0, int y0, int x1, int y1, int thickness, Uint32 col);

void SurfaceDrawCircle(SDL_Surface *surface, int x, int y, int r, Uint32 col);
void SurfaceFillCircle(SDL_Surface *surface, int x, int y, int r, Uint32 col);


#endif