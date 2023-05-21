#include "PopupMenu.h"

#include <SDL.h>

#include "../../Core/Ressources.h"
#include "../../Core/RunState.h"
#include "../../Core/Tileset.h"
#include "../../Globals.h"
#include "Widget.h"

#include <stdio.h>


PopupMenu::PopupMenu(int originx, int originy, int width, int height) {
    m_content = NULL;
    m_originx = originx;
    m_originy = originy;

    m_width = width;
    m_height = height;
    CalculateEdgesPos();
    SetupContent();

    m_animationspeed = 8;
    //Start the animation at the origin of the popup menu
    m_animationbottomrightx = m_originx;
    m_animationbottomrighty = m_originy;
    m_animationtopleftx = m_originx;
    m_animationtoplefty = m_originy;
}

PopupMenu::~PopupMenu() {
    for(Widget* i : m_widgets) {
        delete i;
    }
    
    if(m_content != NULL) {
        SDL_DestroyTexture(m_content);
        m_content = NULL;
    }
}

void PopupMenu::Update() {
    UpdateAnimation();
    
    for(int i = m_widgets.size()-1; i >= 0; i--) m_widgets[i]->Update();

    if(IsMouseOvering()) g_runstate->mouseused = true;


    //Draw to the texture of the menu (the content)
    SDL_SetRenderTarget(g_runstate->renderer, m_content);

    //Clear the texture
    SDL_SetRenderDrawColor(g_runstate->renderer, 0, 0, 0, 0);
    SDL_RenderClear(g_runstate->renderer);
    
    //Draw the menu's children
    for(Widget* i : m_widgets) i->Render();

    //Draw back to the window
    SDL_SetRenderTarget(g_runstate->renderer, NULL);
}

void PopupMenu::Render(SDL_Renderer* renderer) {
    RenderPopup(renderer);

    SDL_FRect src = {
        0,
        0,
        (float)(m_animationbottomrightx - m_animationtopleftx),
        (float)(m_animationbottomrighty - m_animationtoplefty)
    };
    SDL_FRect dest = {
        (float)(m_animationtopleftx),
        (float)(m_animationtoplefty),
        (float)(m_animationbottomrightx - m_animationtopleftx),
        (float)(m_animationbottomrighty - m_animationtoplefty)
    };

    SDL_RenderTexture(renderer, m_content, &src, &dest);
}


void PopupMenu::SetPosition(int x, int y) {
    SetX(x);
    SetY(y);
}
void PopupMenu::SetX(int x) {
    int xoffset = x - m_originx;
    m_originx = x;

    //We need to move all the corner with the origin
    m_topleftx += xoffset;
    m_bottomrightx += xoffset;
    m_animationtopleftx += xoffset;
    m_animationbottomrightx += xoffset;
}
void PopupMenu::SetY(int y) {
    int yoffset = y - m_originy;
    m_originy = y;

    //We need to move all the corner with the origin
    m_toplefty += yoffset;
    m_bottomrighty += yoffset;
    m_animationtoplefty += yoffset;
    m_animationbottomrighty += yoffset;
}

void PopupMenu::SetSize(int w, int h) {
    m_width = w;
    m_height = h;
    CalculateEdgesPos();
    SetupContent();
}
void PopupMenu::SetWidth(int w) {
    m_width = w;
    CalculateEdgesPos();
    SetupContent();
}
void PopupMenu::SetHeight(int h) {
    m_width = h;
    CalculateEdgesPos();
    SetupContent();
}


void PopupMenu::GetAbsoluteSize(int* x, int* y) {
    *x = GetAbsoluteWidth();
    *y = GetAbsoluteHeight();
}

int PopupMenu::GetAbsoluteWidth() {
    return m_bottomrightx - m_topleftx;
}

int PopupMenu::GetAbsoluteHeight() {
    return m_bottomrighty - m_toplefty;
}



void PopupMenu::GetRelativeMousePos(int* x, int* y) {
    *x = g_runstate->mousex - m_animationtopleftx;
    *y = g_runstate->mousey - m_animationtoplefty;
}

bool PopupMenu::IsMouseOvering() {
    //return (g_runstate->mousex >= m_topleftx && g_runstate->mousex < m_bottomrightx
    //     && g_runstate->mousey >= m_toplefty && g_runstate->mousey < m_bottomrighty);
    return (g_runstate->mousex >= m_animationtopleftx && g_runstate->mousex < m_animationbottomrightx
         && g_runstate->mousey >= m_animationtoplefty && g_runstate->mousey < m_animationbottomrighty);
}

void PopupMenu::AddWidget(Widget* item) {
    if(item != NULL) m_widgets.push_back(item);
}

void PopupMenu::SetupContent() {
    if(m_content != NULL) SDL_DestroyTexture(m_content);
    m_content = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, GetAbsoluteWidth(), GetAbsoluteHeight());
    SDL_SetTextureBlendMode(m_content, SDL_BLENDMODE_BLEND); //To make it transparent somehow ?
}



void PopupMenu::UpdateAnimation() {
    //Move the borders of the animations toward the "absolute" borders
    if(m_animationbottomrightx > m_bottomrightx)        m_animationbottomrightx -= m_animationspeed;
    else if(m_animationbottomrightx < m_bottomrightx)   m_animationbottomrightx += m_animationspeed;

    if(m_animationbottomrighty > m_bottomrighty)        m_animationbottomrighty -= m_animationspeed;
    else if(m_animationbottomrighty < m_bottomrighty)   m_animationbottomrighty += m_animationspeed;

    if(m_animationtopleftx > m_topleftx)        m_animationtopleftx -= m_animationspeed;
    else if(m_animationtopleftx < m_topleftx)   m_animationtopleftx += m_animationspeed;

    if(m_animationtoplefty > m_toplefty)        m_animationtoplefty -= m_animationspeed;
    else if(m_animationtoplefty < m_toplefty)   m_animationtoplefty += m_animationspeed;


    //Check if the values of the animations positions is near their "absolute" position
    if(m_animationbottomrightx > m_bottomrightx-m_animationspeed 
    && m_animationbottomrightx < m_bottomrightx+m_animationspeed)
        m_animationbottomrightx = m_bottomrightx;
    
    if(m_animationbottomrighty > m_bottomrighty-m_animationspeed 
    && m_animationbottomrighty < m_bottomrighty+m_animationspeed)
        m_animationbottomrighty = m_bottomrighty;
    
    if(m_animationtopleftx > m_topleftx-m_animationspeed
    && m_animationtopleftx < m_topleftx+m_animationspeed)
        m_animationtopleftx = m_topleftx;
    
    if(m_animationtoplefty > m_toplefty-m_animationspeed
    && m_animationtoplefty < m_toplefty+m_animationspeed)
        m_animationtoplefty = m_toplefty;
    
}

void PopupMenu::CalculateEdgesPos() {
    m_topleftx = m_originx;
    m_toplefty = m_originy;
    m_bottomrightx = m_originx + m_width;
    m_bottomrighty = m_originy + m_height;

    //If the width or height are negatives, then we need to reclculate the topleft and bottomright's positions
    if(m_width < 0) {
        m_topleftx += m_width;
        m_bottomrightx = m_originx;
    }
    if(m_height < 0) {
        m_toplefty += m_height;
        m_bottomrighty = m_originy;
    }
}

void PopupMenu::RenderPopup(SDL_Renderer* renderer) {
    //g_ressources->tileset_popupmenu->DrawRectangle(renderer, m_topleftx, m_toplefty, m_bottomrightx, m_bottomrighty, &popuprectdata);
    g_ressources->tileset_popupmenu->DrawRectangle(renderer, m_animationtopleftx, m_animationtoplefty, m_animationbottomrightx, m_animationbottomrighty, g_ressources->rectdata_popupmenu);
}


float PopupMenu::WidthWithoutBorder() {
    return (float)(m_bottomrightx-m_topleftx-32);
}

float PopupMenu::HeightWithoutBorder() {
    return (float)(m_bottomrighty-m_toplefty-32);
}
