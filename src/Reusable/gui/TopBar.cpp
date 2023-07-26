#include "TopBar.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>

#include "../DeltaTime.h"
#include "../Ressources.h"
#include "../RunState.h"
#include "../Utils.h"

TopBar::TopBar(std::string text, SDL_Color bg, SDL_Color fg) {
    m_mouseonclosebutton = false;
    m_mouseonmaximize = false;
    m_mouseonbar = false;
    m_y = 0;

    m_bg = new SDL_Color(bg);
    m_fg = new SDL_Color(fg);
    m_text = text;

    m_titletexttexturebg = nullptr;
    m_titletexttexturefg = nullptr;

    UpdateTextures();
}

TopBar::~TopBar() {
    SDL_DestroyTexture(m_titletexttexturefg);
    SDL_DestroyTexture(m_titletexttexturebg);

    delete m_bg;
    delete m_fg;
}

void TopBar::Update() {
    //Check if mouse is overring
    m_mouseonbar = g_runstate->mousey < 30;
    m_mouseonclosebutton = (m_mouseonbar && g_runstate->mousex >= g_runstate->winwidth-30);
    m_mouseonmaximize = (m_mouseonbar && g_runstate->mousex >= g_runstate->winwidth-60 && g_runstate->mousex < g_runstate->winwidth-30);

    //Cool animation
    if(m_mouseonbar && m_y < 0) m_y+=(300.0*g_deltatime);
    else if(!m_mouseonbar && m_y > -30) m_y-=(300.0*g_deltatime);

    //Make sure it doesn't go too far
    if(m_y > 0) m_y = 0;


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

void TopBar::Render() {
    SDL_Renderer* renderer = g_runstate->renderer;
    
    //static title (background color)
    const SDL_FRect orangetitlerecdest = {32, 0, (float)m_titlewidth, (float)m_titleheight};
    SDL_RenderTexture(renderer, m_titletexttexturebg, NULL, &orangetitlerecdest);


    SDL_SetRenderDrawColor(renderer, m_bg->r, m_bg->g, m_bg->b, m_bg->a);
    SDL_FRect rec = {0, (float)m_y, (float)g_runstate->winwidth, 30};
    SDL_RenderFillRect(renderer, &rec);


    //Separator (dynamic)
    SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);   //gray
    SDL_RenderLine(renderer, 0, (float)(m_y + 30), g_runstate->winwidth, (float)(m_y + 30));

    //dynamic title (foreground color)
    const SDL_FRect titlerecdest = {32, (float)m_y, (float)m_titlewidth, (float)m_titleheight};
    SDL_RenderTexture(renderer, m_titletexttexturefg, NULL, &titlerecdest);



    ////Title bar buttons

    //Red on close button
    SDL_FRect closerec = {(float)(g_runstate->winwidth-30), 0, 30, 30};
    if(m_mouseonclosebutton) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);   //red
        SDL_RenderFillRect(renderer, &closerec);
    }
    //Close button
    SDL_RenderTexture(renderer, g_reusableressources->txtr_cross, NULL, &closerec);


    //Maximise
    SDL_FRect maximiserec = {(float)(g_runstate->winwidth-60), 0, 30, 30};
    if(m_mouseonmaximize) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 75);
        //TODO ? : maybe it would be usefull to AWLAYS use SDL_BLENDMODE_BLEND to draw everything (and not only that)
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &maximiserec);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
    SDL_RenderTexture(renderer, g_reusableressources->txtr_maximize, NULL, &maximiserec);
}


void TopBar::SetColor(SDL_Color bg, SDL_Color fg) {
    *m_bg = bg;
    *m_fg = fg;
    UpdateTextures();
}

void TopBar::SetText(std::string text) {
    m_text = text;
    UpdateTextures();
}


///////////////////////
// PRIVATE

void TopBar::UpdateTextures() {
    if(m_titletexttexturefg != nullptr) SDL_DestroyTexture(m_titletexttexturefg);
    if(m_titletexttexturebg != nullptr) SDL_DestroyTexture(m_titletexttexturebg);

    m_titletexttexturefg = MakeTextTexture(m_text.c_str(), g_reusableressources->font_ubuntumedium24, *m_fg);
    m_titletexttexturebg = MakeTextTexture(m_text.c_str(), g_reusableressources->font_ubuntumedium24, *m_bg);

    //Get size of texture in pixel
    SDL_QueryTexture(m_titletexttexturefg, NULL, NULL, &m_titlewidth, &m_titleheight);
}
