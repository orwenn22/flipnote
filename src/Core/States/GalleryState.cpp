#include "GalleryState.h"

#include <SDL.h>

#include "../../Reusable/gui/Background.h"
#include "../../Reusable/gui/Label.h"
#include "../../Reusable/gui/TopBar.h"
#include "../../Reusable/State/StateManager.h"
#include "../../Reusable/Utils.h"
#include "../FlipnoteRessources.h"
#include "MainMenuState.h"


GalleryState::GalleryState() {
    m_background = new Background(g_ressources->txtr_bgtile);
    m_topbar = new TopBar("Flipnote gallery", *g_ressources->col_green, *g_ressources->col_white);

    m_cwd = GetCWD();
    m_pathtext = new Label("current path : " + m_cwd, g_ressources->font_ubuntumedium16, *g_ressources->col_green, 45, 60, WidgetAllign_None);

    m_backtext = new Label("Back to main menu", g_ressources->font_ubuntumedium24, *g_ressources->col_green, 45, 34, WidgetAllign_None, [&](){ m_manager->SetState(new MainMenuState());} );
}

GalleryState::~GalleryState() {
    delete m_background;
    delete m_topbar;

    delete m_backtext;
    delete m_pathtext;
}

void GalleryState::Update() {
    m_topbar->Update();

    m_backtext->Update();
    m_pathtext->Update();

    m_background->Update();
}

void GalleryState::Render() {
    m_background->Render();

    m_pathtext->Render();
    m_backtext->Render();

    m_topbar->Render();
}