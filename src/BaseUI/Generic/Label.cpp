#include "Label.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include "../../Core/Ressources.h"
#include "../../Core/RunState.h"
#include "../../Globals.h"

Label::Label(PopupMenu* popupmenu, int x, int y, std::string text) : PopupMenuItem(popupmenu) {
    m_x = x;
    m_y = y;

    SDL_Surface* tmp = TTF_RenderText_Solid(g_ressources->font_ubuntumedium16, text.c_str(), *(g_ressources->col_orange));
    m_texture = SDL_CreateTextureFromSurface(g_runstate->renderer, tmp);
    SDL_DestroySurface(tmp);

    SDL_QueryTexture(m_texture, NULL, NULL, &m_w, &m_h);
}

Label::~Label() {
    SDL_DestroyTexture(m_texture);
}

void Label::Update() {}

void Label::Render() {
    SDL_FRect dest = {(float)m_x,(float)m_y,(float)m_w,(float)m_h};
    SDL_RenderTexture(g_runstate->renderer, m_texture, NULL, &dest);
}
