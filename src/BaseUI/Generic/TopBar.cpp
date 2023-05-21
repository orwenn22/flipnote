#include "TopBar.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include "../../Core/Ressources.h"
#include "../../Core/RunState.h"
#include "../../Globals.h"

TopBar::TopBar(SDL_Renderer* renderer) {
    m_mouseonclosebutton = false;
    m_mouseonmaximize = false;
    m_mouseonbar = false;
    m_y = 0;

    SDL_Surface* titletextsurface = TTF_RenderText_Solid(g_ressources->font_ubuntumedium24, "Flipnote", *g_ressources->col_white);    //Write text to surface (white)
    m_titletexttexturewhite = SDL_CreateTextureFromSurface(renderer, titletextsurface);     //copy surface to texture
    SDL_QueryTexture(m_titletexttexturewhite, NULL, NULL, &m_titlewidth, &m_titleheight);   //Get size of texture in pixel
    SDL_DestroySurface(titletextsurface);           //Deallocate the surface as it is now useless

    titletextsurface = TTF_RenderText_Solid(g_ressources->font_ubuntumedium24, "Flipnote", *g_ressources->col_orange);    //Write text to surface (orange)
    m_titletexttextureorange = SDL_CreateTextureFromSurface(renderer, titletextsurface);     //copy surface to texture
    SDL_DestroySurface(titletextsurface);           //Deallocate the surface as it is now useless
}

TopBar::~TopBar() {
    SDL_DestroyTexture(m_titletexttexturewhite);
    SDL_DestroyTexture(m_titletexttextureorange);
}

void TopBar::Update() {
    //Check if mouse is overring
    m_mouseonbar = g_runstate->mousey < 30;
    m_mouseonclosebutton = (m_mouseonbar && g_runstate->mousex >= g_runstate->winwidth-30);
    m_mouseonmaximize = (m_mouseonbar && g_runstate->mousex >= g_runstate->winwidth-60 && g_runstate->mousex < g_runstate->winwidth-30);

    //Cool animation
    if(m_mouseonbar && m_y < 0) m_y+=5;
    else if(!m_mouseonbar && m_y > -30) m_y-=5;


    //Don't check button clicks if the mouse have already been used
    if(g_runstate->mouseused) return;

    //Title bar buttons
    if(g_runstate->leftclick) {
        //Close button
        if(m_mouseonclosebutton) {
            g_runstate->running = false;
        }
        //Maximize
        else if(m_mouseonmaximize) {
            if((SDL_GetWindowFlags(g_runstate->window) & SDL_WINDOW_MAXIMIZED) == 0) {
                SDL_MaximizeWindow(g_runstate->window);
            }
            else {
                SDL_RestoreWindow(g_runstate->window);
            }
        }
    }

    //Make sure the mouse don't interact with stuff below the topbar
    if(m_mouseonbar) {
        g_runstate->mouseused = true;
    }
}

void TopBar::Render(SDL_Renderer* renderer) {
    //orange title (static)
    const SDL_FRect orangetitlerecdest = {32, 0, (float)m_titlewidth, (float)m_titleheight};
    SDL_RenderTexture(renderer, m_titletexttextureorange, NULL, &orangetitlerecdest);


    //Orange top bar (dynamic)
    SDL_Color* orange = g_ressources->col_orange;
    SDL_SetRenderDrawColor(renderer, orange->r, orange->g, orange->b, orange->a);
    SDL_FRect rec = {0, (float)m_y, (float)g_runstate->winwidth, 30};
    SDL_RenderFillRect(renderer, &rec);


    //Separator (dynamic)
    SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);   //gray
    SDL_RenderLine(renderer, 0, (float)(m_y + 30), g_runstate->winwidth, (float)(m_y + 30));

    //white title (dynamic)
    const SDL_FRect titlerecdest = {32, (float)m_y, (float)m_titlewidth, (float)m_titleheight};
    SDL_RenderTexture(renderer, m_titletexttexturewhite, NULL, &titlerecdest);



    ////Title bar buttons

    //Red on close button
    SDL_FRect closerec = {(float)(g_runstate->winwidth-30), 0, 30, 30};
    if(m_mouseonclosebutton) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);   //red
        SDL_RenderFillRect(renderer, &closerec);
    }
    //Close button
    SDL_RenderTexture(renderer, g_ressources->txtr_cross, NULL, &closerec);


    //Maximise
    SDL_FRect maximiserec = {(float)(g_runstate->winwidth-60), 0, 30, 30};
    if(m_mouseonmaximize) {
        SDL_SetRenderDrawColor(renderer, 255, 120, 50, 255);   //orange (but a little diferent)
        SDL_RenderFillRect(renderer, &maximiserec);
    }
    SDL_RenderTexture(renderer, g_ressources->txtr_maximize, NULL, &maximiserec);
}