#include "Label.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>

#include "../Ressources.h"
#include "../RunState.h"
#include "../Utils.h"

Label::Label(
    std::string text, _TTF_Font* font, SDL_Color color,
    int x, int y, WidgetAllign allignment, 
    ClickabbleWidgetCallback callback
) : ClickableWidget(x, y, 0, 0, allignment) {
    m_texture = MakeTextTexture(text.c_str(), font, color);

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
