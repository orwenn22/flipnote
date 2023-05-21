#include "FlipnotePainter.h"

#include <math.h>
#include <SDL.h>

#include "../../Globals.h"
#include "../RunState.h"
#include "FlipnoteFrame.h"

#include <stdio.h>

bool DefaultPaintCondition(int x, int y) {
    return true;

    //lignes verticales et horizontales
    //return x%2;
    //return y%2;

    //Diagonales
    //return (x-y)%3 == 0;
    //return (x-y)%4 == 0;
    //return (x+y)%3 == 0;
    //return (x+y)%4 == 0;


    // "cadriage"
    //return (x%2 && y%2);
    //return !(x%2 && y%2);
    //return (x%3 && y%3);
    //return !(x%3 && y%3);

    //return (x-y)%2 == 0 || (x+y)%2 == 0;
    //return (x-y)%3 == 0 || (x+y)%3 == 0;
    

    //weird brushes
    //return (x-y)%3 == 0 || (x+y)%2 == 0;
    //return (x-y)%3 == 0 || (x+y)%4 == 3;
    //return (x-y)%4 == 0 || (x+y)%4 == 0;

    //rectangles
    //return (x%2 && y%3);
    //return (x%3 && y%2);
}

FlipnotePainter::FlipnotePainter(FlipnoteFrame* frame, SDL_Texture* texture, PaintCondition paintcondition, bool invertpaint) {
    m_frame = frame;
    m_texture = texture;
    m_paintcondition = paintcondition;
    m_invertpaint = invertpaint;
}


void FlipnotePainter::SetPixel(int x, int y, int colorindex) {
    SDL_Texture* previoustarget = PrepareRender(colorindex);

    InternalSetPixel(x, y, colorindex);

    SDL_SetRenderTarget(g_runstate->renderer, previoustarget);
}


void FlipnotePainter::DrawLine(int x0, int y0, int x1, int y1, int colorindex) {
    SDL_Texture* previoustarget = PrepareRender(colorindex);

    int i;
    double x = x1 - x0; 
	double y = y1 - y0; 
	double length = sqrt( x*x + y*y ); 
	double addx = x / length; 
	double addy = y / length; 
	x = x0; 
	y = y0; 
	
	for ( i = 0; i < length; i += 1) { 
        InternalSetPixel(x, y, colorindex);
		x += addx; 
		y += addy; 
		
	} 

    SDL_SetRenderTarget(g_runstate->renderer, previoustarget);
}

void FlipnotePainter::DrawLineEx(int x0, int y0, int x1, int y1, int thickness, int colorindex) {
    SDL_Texture* previoustarget = PrepareRender(colorindex);

    int i;
    double x = x1 - x0; 
	double y = y1 - y0; 
	double length = sqrt( x*x + y*y ); 
	double addx = x / length; 
	double addy = y / length; 
	x = x0; 
	y = y0; 
	
    InternalFillCircle(x, y, thickness, colorindex);
	for ( i = 0; i < length; i += 1) { 
		InternalDrawCircle(x, y, thickness, colorindex);
        InternalDrawCircle(x, y, thickness-1, colorindex);  //without this there are pixel that are not being painted
		x += addx; 
		y += addy; 
		
	} 

    SDL_SetRenderTarget(g_runstate->renderer, previoustarget);
}

void FlipnotePainter::DrawCircle(int x, int y, int r, int colorindex) {
    SDL_Texture* previoustarget = PrepareRender(colorindex);

    InternalDrawCircle(x, y, r, colorindex);

    SDL_SetRenderTarget(g_runstate->renderer, previoustarget);
}


void FlipnotePainter::FillCircle(int x, int y, int r, int colorindex) {
    SDL_Texture* previoustarget = PrepareRender(colorindex);

    InternalFillCircle(x, y, r, colorindex);

    SDL_SetRenderTarget(g_runstate->renderer, previoustarget);
}

/////////////////
// Private

void FlipnotePainter::InternalSetPixel(int x, int y, int colorindex) {
    if(m_paintcondition(x, y) ^ m_invertpaint) {
        if(m_frame) m_frame->SetPixel(x, y, colorindex);    //don't draw on the FlipnoteFrame if it is not set
        SDL_RenderPoint(g_runstate->renderer, x, y);
    }
}

void FlipnotePainter::InternalDrawCircle(int x, int y, int r, int colorindex) {
    if(r<0)return;

    //fun fact : before there wass a mistake here : step = (M_PI*2)/(r*r*M_PI)
    //which caused the function to be super slow lol
    
    //float step = (M_PI*2)/(2*r*M_PI);
    float step = 1.0f/r;
	step /= 4;


    //printf("wow\n%f\n", (M_PI/2)/step);

	//for(float i = 0; i < M_PI*2; i+= step) {
    for(float i = 0; i < M_PI/2; i+= step) {
		float c = cos(i) * (float)r;
		float s = sin(i) * (float)r;

		InternalSetPixel(x+(int)round(c), y+(int)round(s), colorindex);
        InternalSetPixel(x-(int)round(c), y+(int)round(s), colorindex);
        InternalSetPixel(x+(int)round(c), y-(int)round(s), colorindex);
        InternalSetPixel(x-(int)round(c), y-(int)round(s), colorindex);
	}
}

void FlipnotePainter::InternalFillCircle(int x, int y, int r, int colorindex) {
    InternalSetPixel(x, y, colorindex);
    for(int current_r = 1; current_r <= r; current_r++) {
		InternalDrawCircle(x, y, current_r, colorindex);
	}
}



SDL_Texture* FlipnotePainter::PrepareRender(int colorindex) {
    SDL_Texture* previoustarget = SDL_GetRenderTarget(g_runstate->renderer);

    SDL_SetRenderTarget(g_runstate->renderer, m_texture);

    SDL_Color c;
    if(m_frame) c = m_frame->GetColor(colorindex);
    else c = FlipnoteFrame::GetDefaultColor(colorindex); 

    SDL_SetRenderDrawColor(g_runstate->renderer, c.r, c.g, c.b, c.a);

    return previoustarget;
}