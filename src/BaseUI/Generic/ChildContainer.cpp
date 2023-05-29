#include "ChildContainer.h"
#include "../../Globals.h"
#include "../../Core/RunState.h"


#include <SDL.h>
#include <stdio.h>

ChildContainer::ChildContainer(
    WidgetContainer* parrent, 
    int x, int y, int w, int h, 
    WidgetAllign allignment
) : WidgetContainer() , Widget(parrent, x, y, w, h, allignment) {
    m_issolid = false;

    if(w < 0 || h < 0) {
        m_texture = NULL;
    }
    else {
        m_texture = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, w, h);
        SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
    }

    if(parrent == NULL) {
        printf("ChildContainer don't have parrent\n");
    }
}

ChildContainer::~ChildContainer() {
    if(m_texture != NULL) {
        SDL_DestroyTexture(m_texture);
    }
}

void ChildContainer::Update() {
    //Save the previous rendertarget
    SDL_Texture* oldrendertarget = SDL_GetRenderTarget(g_runstate->renderer);

    //Update all widgets (update pos, react to userinput, etc)
    UpdateWidgets();

    //Draw all the widgets on the texture
    SDL_SetRenderTarget(g_runstate->renderer, m_texture);
    SDL_SetRenderDrawColor(g_runstate->renderer, 0, 0, 0, 0);
    SDL_RenderClear(g_runstate->renderer);
    RenderWidgets();
    SDL_SetRenderTarget(g_runstate->renderer, oldrendertarget);

    if(IsOvered() && m_issolid) {
        g_runstate->mouseused = true;
    }
}

void ChildContainer::Render() {
    //Draw the texture (where all the widget are drawn) on the screen
    SDL_FRect dest = {(float)GetX(), (float)GetY(), (float)m_w, (float)m_h};
    SDL_SetRenderDrawColor(g_runstate->renderer, 128, 128, 128, 255);
    SDL_RenderRect(g_runstate->renderer, &dest);
    SDL_RenderTexture(g_runstate->renderer, m_texture, NULL, &dest);
}


int ChildContainer::GetContainerWidth() {
    return m_w;
}
int ChildContainer::GetContainerHeight() {
    return m_h;
}

int ChildContainer::GetContainerX() { return GetAbsoluteX(); }
int ChildContainer::GetContainerY() { return GetAbsoluteY(); }


bool ChildContainer::IsMouseOvering() {
    return IsOvered();
}

void ChildContainer::GetRelativeMousePos(int* x, int* y) {
    m_container->GetRelativeMousePos(x, y);
    *x -= GetX();
    *y -= GetY();
}