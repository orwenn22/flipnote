#include "Label.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include "../Globals.h"
#include "../Ressources.h"
#include "../RunState.h"

Label::Label(WidgetContainer* container, int x, int y, std::string text, ClickabbleWidgetCallback callback) 
: ClickableWidget(container, x, y) {
    SDL_Surface* tmp = TTF_RenderText_Solid(g_reusableressources->font_ubuntumedium16, text.c_str(), *(g_reusableressources->col_orange));
    m_texture = SDL_CreateTextureFromSurface(g_runstate->renderer, tmp);
    SDL_DestroySurface(tmp);

    SDL_QueryTexture(m_texture, NULL, NULL, &m_w, &m_h);
    m_callback = callback;
}

Label::~Label() {
    SDL_DestroyTexture(m_texture);
}

void Label::Render() {
    SDL_FRect dest = {(float)GetX(),(float)GetY(),(float)m_w,(float)m_h};
    SDL_RenderTexture(g_runstate->renderer, m_texture, NULL, &dest);
}
