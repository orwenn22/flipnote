#include "IconButton.h"

#include <SDL3/SDL.h>

#include "../RunState.h"

IconButton::IconButton(
    SDL_Texture* texture,
    int x, int y, int w, int h,
    WidgetAllign allignment,
    ClickabbleWidgetCallback callback
)
: ClickableWidget(x, y, w, h, allignment, callback) {
    m_texture = texture;
    if(m_texture == NULL) return;
    
    SDL_QueryTexture(m_texture, NULL, NULL, &m_w, &m_h);
    UpdatePos();
}

IconButton::~IconButton() {

}


void IconButton::Render() {
    SDL_FRect dest = {(float)GetX(), (float)GetY(), (float)m_w, (float)m_h};
    SDL_RenderTexture(g_runstate->renderer, m_texture, NULL, &dest);
}
