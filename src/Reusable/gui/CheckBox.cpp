#include "CheckBox.h"

#include <SDL.h>

#include "../Globals.h"
#include "../Ressources.h"
#include "../RunState.h"
#include "Label.h"
#include "WidgetContainer.h"


CheckBox::CheckBox(WidgetContainer* container, int x, int y, bool* target, std::string label, WidgetAllign allignment) 
: ClickableWidget(container, x, y, 16, 16, allignment) {
    m_target = target;
    
    m_callback = [&]() -> void {
        if(g_runstate->leftclick) {
            *m_target = !*m_target;
        }
    };

    if(!label.empty())
        container->AddWidget(new Label(container, label, x+20, y, allignment, m_callback));
}

void CheckBox::Render() {
    SDL_FRect dest = {(float)GetX(), (float)GetY(), (float)m_w, (float)m_h};
    SDL_Color *orange = g_reusableressources->col_orange;

    if(*m_target) {
        SDL_SetTextureColorMod(g_reusableressources->txtr_checkboxtrue, orange->r, orange->g, orange->b);
        SDL_RenderTexture(g_runstate->renderer, g_reusableressources->txtr_checkboxtrue, NULL, &dest);
    } 
    else {
        SDL_SetTextureColorMod(g_reusableressources->txtr_checkboxfalse, orange->r, orange->g, orange->b);
        SDL_RenderTexture(g_runstate->renderer, g_reusableressources->txtr_checkboxfalse, NULL, &dest);
    }
}
