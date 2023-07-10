#include "FrogMenuButton.h"

#include <SDL.h>

#include "../../../Core/FlipnoteRessources.h"
#include "../../../Reusable/gui/Label.h"
#include "../../../Reusable/gui/PopupMenu.h"
#include "../../../Reusable/RunState.h"
#include "../FlipnoteEditor.h"
#include "../Menu/MenuAligner.h"

#include <stdio.h>

FrogMenuButton::FrogMenuButton(
    WidgetContainer* container, FlipnoteEditor* editor,
    int x, int y, WidgetAllign allignment
) : EditorButton(container, editor, x, y, 0, 0, allignment) {
    SDL_QueryTexture(g_ressources->txtr_button_frog, NULL, NULL, &m_w, &m_h);

    m_callback = [&]()  {
        PopupMenu* pm = new PopupMenu(GetX(), GetY()-10, 200, -200);

        pm->AddWidget(new MenuAligner(pm, this, 0, -10));
        pm->AddWidget(new Label(pm, "Frog Menu", 5, 5, WidgetAllign_None));
        

        m_editor->OpenPopupMenu(pm);
    };
}

void FrogMenuButton::Render() {
    SDL_FRect dest = {(float)GetX(), (float)GetY(), m_w, m_h};
    SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_button_frog, NULL, &dest);
}
