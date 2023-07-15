#include "SpeedSetting.h"

#include <SDL.h>

#include "../../../Core/FlipnoteRessources.h"
#include "../../../Core/Flipnote/Flipnote.h"
#include "../../../Reusable/gui/WidgetContainer.h"
#include "../../../Reusable/RunState.h"


//Width in pixel of an arrow
static int s_arrow_width = -1;
static int s_arrow_height = -1;


SpeedSetting::SpeedSetting(Flipnote* flipnote, int x, int y, WidgetAllign allignment)
: Widget(x, y, 0, 0, allignment) {
    m_flipnote = flipnote;

    SDL_QueryTexture(g_ressources->txtr_icon_red_arrow, NULL, NULL, &s_arrow_width, &s_arrow_height);
    m_w = s_arrow_width*11;     //11 speeds are available
    m_h = s_arrow_height;
}

void SpeedSetting::Update() {
    UpdatePos();

    if(g_runstate->mouseused) return;

    //This is for when the popupmenu is not fully deployed (the animation is not over)
    if(!m_container->IsMouseOvering()) return;

    if(IsOvered()) {
        g_runstate->mouseused = true;

        if(g_runstate->leftpressed) {
            int mousex, mousey;
            m_container->GetRelativeMousePos(&mousex, &mousey);

            int relativemousex = mousex - GetX();
            int overredindex = relativemousex/s_arrow_width;

            m_flipnote->SetAnimationSpeed(overredindex);
        }
    }
}

void SpeedSetting::Render() {
    int currentspeed = m_flipnote->GetAnimationSpeed();

    SDL_FRect dest = {(float)GetX(), (float)GetY(), (float)s_arrow_width, (float)s_arrow_height};

    int i = 0;
    for( ; i <= currentspeed; i++) {
        SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_icon_red_arrow, NULL, &dest);
        dest.x += (float)s_arrow_width;
    }

    for( ; i < 11; i++) {
        SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_icon_yellow_arrow, NULL, &dest);
        dest.x += (float)s_arrow_width;
    }
}
