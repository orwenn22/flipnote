#include "CheckBox.h"

#include <SDL.h>

#include "../../Core/Ressources.h"
#include "../../Core/RunState.h"
#include "../../Globals.h"
#include "Label.h"
#include "WidgetContainer.h"


CheckBox::CheckBox(WidgetContainer* container, int x, int y, bool* target, std::string label) : Widget(container, x, y, 16, 16) {
    m_target = target;

    if(!label.empty())
        container->AddWidget(new Label(container, x+20, y, label));
}

void CheckBox::Update() {
    //This is for when the menu is not fully deployed (the animation is not over)
    if(!m_container->IsMouseOvering()) return;

    if(g_runstate->mouseused) return;

    if(IsOvered()) {
        g_runstate->mouseused = true;
        
        if(g_runstate->leftclick) {
            *m_target = !*m_target;
        }
    }
}

void CheckBox::Render() {
    SDL_FRect dest = {(float)GetX(), (float)GetY(), (float)m_w, (float)m_h};
    SDL_Color *orange = g_ressources->col_orange;

    if(*m_target) {
        SDL_SetTextureColorMod(g_ressources->txtr_checkboxtrue, orange->r, orange->g, orange->b);
        SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_checkboxtrue, NULL, &dest);
    } 
    else {
        SDL_SetTextureColorMod(g_ressources->txtr_checkboxfalse, orange->r, orange->g, orange->b);
        SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_checkboxfalse, NULL, &dest);
    }
}
