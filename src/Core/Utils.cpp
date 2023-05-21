#include "Utils.h"

#include <math.h>

Uint32 SDLColorToUint32(SDL_Color c) {
    return ((c.a << 24) | (c.r << 16) | (c.g << 8) | c.b);
}

void SurfaceSetPixel(SDL_Surface* surface, int x, int y, Uint32 col) {
    if(x < 0 || x >= surface->w || y < 0 || y >= surface->h)
        return;

    //from https://stackoverflow.com/questions/20070155/how-to-set-a-pixel-in-a-sdl-surface
    Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels
                                             + y * surface->pitch
                                             + x * surface->format->BytesPerPixel);
    
    *target_pixel = col;
}

void SurfaceDrawLine(SDL_Surface *surface, int x0, int y0, int x1, int y1, Uint32 col) {
    int i;
    double x = x1 - x0; 
	double y = y1 - y0; 
	double length = sqrt( x*x + y*y ); 
	double addx = x / length; 
	double addy = y / length; 
	x = x0; 
	y = y0; 
	
	for ( i = 0; i < length; i += 1) { 
		SurfaceSetPixel(surface, x, y, col); 
		x += addx; 
		y += addy; 
		
	} 
}

void SurfaceDrawLineEx(SDL_Surface *surface, int x0, int y0, int x1, int y1, int thickness, Uint32 col) {
	int i;
    double x = x1 - x0; 
	double y = y1 - y0; 
	double length = sqrt( x*x + y*y ); 
	double addx = x / length; 
	double addy = y / length; 
	x = x0; 
	y = y0; 
	
	for ( i = 0; i < length; i += 1) { 
		SurfaceFillCircle(surface, x, y, thickness, col);
		x += addx; 
		y += addy; 
		
	} 
}



void SurfaceDrawCircle(SDL_Surface *surface, int x, int y, int r, Uint32 col) {
	float step = (2.0f) / (r*r);	//(PI*2)/(r*r*PI)
	step /= 4;
	for(float i = 0; i < M_PI*2; i+= step) {
		float c = cos(i) * (float)r;
		float s = sin(i) * (float)r;

		
		SurfaceSetPixel(surface, x+(int)round(c), y+(int)round(s), col);
	}
}

void SurfaceFillCircle(SDL_Surface *surface, int x, int y, int r, Uint32 col) {
	for(int current_r = 1; current_r <= r; current_r++) {
		SurfaceDrawCircle(surface, x, y, current_r, col);
	}
}