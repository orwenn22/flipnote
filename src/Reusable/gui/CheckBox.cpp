#include "CheckBox.h"

#include <SDL3/SDL.h>

#include "../Ressources.h"
#include "../RunState.h"
#include "Label.h"
#include "WidgetContainer.h"


CheckBox::CheckBox(int x, int y, bool* target, std::string label, WidgetAllign allignment) 
: ClickableWidget(x, y, 16, 16, allignment) {
    m_target = target;
    
    m_callback = [&]() -> void {
        if(g_runstate->leftclick) {
            *m_target = !*m_target;
        }
    };

    if(!label.empty()) m_label = new Label(label, g_reusableressources->font_ubuntumedium16, *g_reusableressources->col_orange, x+20, y, allignment, m_callback);
    else m_label = nullptr;
}

CheckBox::~CheckBox() {
    if(m_label != nullptr) {
        delete m_label;
        m_label = nullptr;
    }
}

void CheckBox::Render() {
    if(m_label != nullptr) m_label->Render();

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


void CheckBox::OnContainerAdd() {
    if(m_label == nullptr) return;

    m_container->AddWidget(m_label);
    m_label = nullptr;
}
