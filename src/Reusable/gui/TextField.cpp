#include "TextField.h"

#include "../Ressources.h"
#include "../RunState.h"
#include "../Tileset.h"
#include "../Utils.h"
#include "WidgetContainer.h"

TextField::TextField(std::string *target, int x, int y, int w, WidgetAllign allignment) : Widget(x, y, w, 26, allignment) {
    m_target = target;
    m_texttexture = nullptr;
    m_selected = false;

    if(m_target != nullptr) {
        m_texttexture = MakeTextTexture(m_target->c_str(), g_reusableressources->font_ubuntumedium16, *g_reusableressources->col_orange);
    }
}

TextField::~TextField() {
    if(m_texttexture != nullptr) {
        SDL_DestroyTexture(m_texttexture);
        m_texttexture = nullptr;
    }
}


void TextField::Update() {
    if(g_runstate->leftclick) m_selected = false;
    if(m_selected) CheckForInput();

    if(g_runstate->mouseused) return;

    //This is for when the popupmenu is not fully deployed (the animation is not over)
    if(m_container != nullptr && !m_container->IsMouseOvering()) return;

    if(IsOvered()) {
        if(g_runstate->leftclick) m_selected = true;
        g_runstate->mouseused = true;
    }
}

void TextField::Render() {
    SDL_FRect dest = {(float)GetX(), (float)GetY(), (float)m_w, (float)m_h};

    SDL_Color* c;
    if (m_selected) c = g_reusableressources->col_red;
    else c = g_reusableressources->col_orange;

    SDL_SetTextureColorMod(g_reusableressources->txtr_rounded_rect_line, c->r, c->g, c->b);
    g_reusableressources->tileset_rounded_rect_line->DrawRectangle(g_runstate->renderer, &dest, g_reusableressources->rectdata_basic_rect);

    if(m_texttexture != nullptr) {
        int w, h;
        SDL_QueryTexture(m_texttexture, nullptr, nullptr, &w, &h);
        dest.x += 3.0f;
        dest.y += 3.0f;
        dest.w = (float)w;
        dest.h = (float)h;
        SDL_RenderTexture(g_runstate->renderer, m_texttexture, nullptr, &dest);
    }
}


void TextField::CheckForInput() {
    if(m_target == nullptr) return;
    bool need_update = false;

    //handle uppercase
    int offset = -32 * g_runstate->IsKeyDown(SDLK_LSHIFT);

    //check for backspace
    if(g_runstate->IsKeyPressed(SDLK_BACKSPACE)) {
        if(!m_target->empty()) {
            m_target->pop_back();
            need_update = true;
        }
    }

    //check for space
    if(g_runstate->IsKeyPressed(SDLK_SPACE)) {
        m_target->push_back(' ');
        need_update = true;
    }

    //check for letters
    for(int k = SDLK_a; k <= SDLK_z; k++) {
        if(g_runstate->IsKeyPressed(k)) {
            m_target->push_back((char)(k+offset));
            need_update = true;
        }
    }

    //check for digits
    for(int k = SDLK_0; k <= SDLK_9; k++) {
        if(g_runstate->IsKeyPressed(k)) {
            m_target->push_back((char)k);
            need_update = true;
        }
    }

    //Update the text's texture if necessary
    if(need_update) {
        if (m_texttexture != nullptr) SDL_DestroyTexture(m_texttexture);
        m_texttexture = MakeTextTexture(m_target->c_str(), g_reusableressources->font_ubuntumedium16,*g_reusableressources->col_orange);
    }
}
