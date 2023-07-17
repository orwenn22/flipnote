#include "FrogMenuButton.h"

#include <SDL.h>

#include "../../../Core/FlipnoteRessources.h"
#include "../../../Reusable/gui/CheckBox.h"
#include "../../../Reusable/gui/Label.h"
#include "../../../Reusable/gui/PopupMenu.h"
#include "../../../Reusable/RunState.h"
#include "../FlipnoteDisplay.h"
#include "../FlipnoteEditor.h"
#include "../FrogMenu/SpeedSetting.h"
#include "../Menu/MenuAligner.h"

#include <stdio.h>

FrogMenuButton::FrogMenuButton(
    FlipnoteEditor* editor,
    int x, int y, WidgetAllign allignment
) : EditorButton(editor, x, y, 0, 0, allignment) {
    SDL_QueryTexture(g_ressources->txtr_button_frog, NULL, NULL, &m_w, &m_h);

    m_callback = [&]()  {
        PopupMenu* pm = new PopupMenu(GetX(), GetY()-10, 220, -200);

        pm->AddWidget(new MenuAligner(this, 0, -10));
        pm->AddWidget(new Label("Frog Menu", g_reusableressources->font_ubuntumedium24, 5, 5, WidgetAllign_None));
        
        pm->AddWidget(new CheckBox(5, 32, m_editor->GetDisplay()->GetShowPreviousFramePreviewPtr(), "Show previous frame"));

        pm->AddWidget(new Label("Animation speed", g_reusableressources->font_ubuntumedium16, 5, 38, WidgetAllign_Bottom));
        pm->AddWidget(new SpeedSetting(m_editor->GetFlipnote(), 5, 5, WidgetAllign_Bottom));
        

        m_editor->OpenPopupMenu(pm);
    };
}

void FrogMenuButton::Render() {
    SDL_FRect dest = {(float)GetX(), (float)GetY(), (float)m_w, (float)m_h};
    SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_button_frog, NULL, &dest);
}
