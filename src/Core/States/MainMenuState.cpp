#include "MainMenuState.h"

#include "../../Reusable/gui/Background.h"
#include "../../Reusable/gui/Label.h"
#include "../../Reusable/gui/TopBar.h"
#include "../../Reusable/gui/WinWidgetContainer.h"
#include "../../Reusable/State/StateManager.h"
#include "../FlipnoteRessources.h"
#include "EditorState.h"
#include "GalleryState.h"

#include <SDL.h>


MainMenuState::MainMenuState() : SimpleState() {
    auto background = new Background(g_ressources->txtr_bgtile);
    auto widgets = new WinWidgetContainer();
    auto topbar = new TopBar("Flipnote", *g_ressources->col_orange, *g_ressources->col_white);

    widgets->AddWidget(new Label("Create new", g_reusableressources->font_ubuntumedium16, *g_reusableressources->col_orange, 0, -10, WidgetAllign_Center,[&](){ m_manager->SetState(new EditorState()); }));
    widgets->AddWidget(new Label("load", g_reusableressources->font_ubuntumedium16, *g_reusableressources->col_orange, 0, 10, WidgetAllign_Center,[&](){ m_manager->SetState(new GalleryState()); }));

    AddComponant(background);
    AddComponant(widgets);
    AddComponant(topbar);
}

MainMenuState::~MainMenuState() {

}
