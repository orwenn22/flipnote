#include "FlipnotePainter.h"

#include <math.h>
#include <SDL.h>

#include "../../Reusable/RunState.h"
#include "FlipnoteFrame.h"
#include "Flipnote.h"

#include <stdio.h>

bool DefaultPaintCondition(int x, int y) {
    return true;
}


FlipnotePainter::FlipnotePainter(SDL_Texture* texture, PaintCondition paintcondition, bool invertpaint) {
    m_frame = nullptr;
    m_texture = texture;
    m_paintcondition = paintcondition;
    m_invertpaint = invertpaint;

    m_palette = Flipnote::GetDefaultPalette();
    m_targetlayer = 0;
}

FlipnotePainter::FlipnotePainter(FlipnoteFrame* frame, int targetlayer, SDL_Texture* texture, PaintCondition paintcondition, bool invertpaint) 
: FlipnotePainter(texture, paintcondition, invertpaint) {
    m_frame = frame;

    if(m_frame == nullptr) m_palette = Flipnote::GetDefaultPalette();
    else m_palette = m_frame->m_flipnote->GetPalette();
    
    m_targetlayer = targetlayer;
}

FlipnotePainter::FlipnotePainter(SDL_Color* palette, SDL_Texture* texture, PaintCondition paintcondition, bool invertpaint) 
: FlipnotePainter(texture, paintcondition, invertpaint) {
    m_palette = palette;

    if(m_palette == nullptr) m_palette = Flipnote::GetDefaultPalette();

    m_targetlayer = 0;
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
        InternalSetPixel(x, y, colorindex);
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
        if(m_frame) m_frame->SetPixel(x, y, m_targetlayer, colorindex);    //don't draw on the FlipnoteFrame if it is not set
        SDL_RenderPoint(g_runstate->renderer, x, y);
    }
}

void FlipnotePainter::InternalDrawCircle(int centerx, int centery, int r, int colorindex) {
    if(r<0)return;

    //float step = (M_PI*2)/(2*r*M_PI);
    float step = 1.0f/r;
    step /= 2;

    for(float i = 0; i < M_PI/2; i+= step) {
		float c = cos(i) * (float)r;
		float s = sin(i) * (float)r;
    
		InternalSetPixel(centerx+(int)round(c), centery+(int)round(s), colorindex);
        InternalSetPixel(centerx-(int)round(c), centery+(int)round(s), colorindex);
        InternalSetPixel(centerx+(int)round(c), centery-(int)round(s), colorindex);
        InternalSetPixel(centerx-(int)round(c), centery-(int)round(s), colorindex);
	}
    
    

    
    /*
    //from https://stackoverflow.com/questions/38334081/how-to-draw-circles-arcs-and-vector-graphics-in-sdl
    const int32_t diameter = (r * 2);

   int32_t x = (r - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      InternalSetPixel(centerx + x, centery - y, colorindex);
      InternalSetPixel(centerx + x, centery + y, colorindex);
      InternalSetPixel(centerx - x, centery - y, colorindex);
      InternalSetPixel(centerx - x, centery + y, colorindex);
      InternalSetPixel(centerx + y, centery - x, colorindex);
      InternalSetPixel(centerx + y, centery + x, colorindex);
      InternalSetPixel(centerx - y, centery - x, colorindex);
      InternalSetPixel(centerx - y, centery + x, colorindex);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
    */
}

void FlipnotePainter::InternalFillCircle(int centerx, int centery, int r, int colorindex) {
    InternalSetPixel(centerx, centery, colorindex);
    for(int current_r = 1; current_r <= r; current_r++) {
		InternalDrawCircle(centerx, centery, current_r, colorindex);
	}

    //r+=1;
    //for (int w = 0; w < r+1; w++) {
    //    for (int h = 0; h < r+1; h++) {
    //        int dx = r - w; // horizontal offset
    //        int dy = r - h; // vertical offset
    //        if ((dx*dx + dy*dy) < (r * r)) {
    //            //One pixel for each "quarter" of the circle
    //            InternalSetPixel(centerx + dx, centery + dy, colorindex);
    //            InternalSetPixel(centerx - dx, centery + dy, colorindex);
    //            InternalSetPixel(centerx + dx, centery - dy, colorindex);
    //            InternalSetPixel(centerx - dx, centery - dy, colorindex);
    //        }
    //    }
    //}
}


SDL_Texture* FlipnotePainter::PrepareRender(int colorindex) {
    SDL_Texture* previoustarget = SDL_GetRenderTarget(g_runstate->renderer);

    SDL_SetRenderDrawBlendMode(g_runstate->renderer, SDL_BLENDMODE_NONE);   //Necessary for the "eraser" effect on the texture
    SDL_SetRenderTarget(g_runstate->renderer, m_texture);

    SDL_Color c;

    if(colorindex == 0 && m_frame != nullptr) {  // drawing with first color (eraser) and a frame is defined
        c = {0, 0, 0, 0};  //transparent / blank / "eraser"
    }
    else {  //not drawing with first color = always fill with color from palette  (or when m_frame is not defined)
        c = m_palette[colorindex];
    }

    SDL_SetRenderDrawColor(g_runstate->renderer, c.r, c.g, c.b, c.a);

    return previoustarget;
}
