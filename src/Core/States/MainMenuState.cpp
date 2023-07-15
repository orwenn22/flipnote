#include "MainMenuState.h"

#include "../../Reusable/gui/Background.h"
#include "../../Reusable/gui/Label.h"
#include "../../Reusable/gui/TopBar.h"
#include "../../Reusable/gui/WinWidgetContainer.h"
#include "../../Reusable/State/StateManager.h"
#include "../FlipnoteRessources.h"
#include "EditorState.h"

#include <SDL.h>


MainMenuState::MainMenuState() : State() {
    m_background = new Background(g_ressources->txtr_bgtile);
    m_topbar = new TopBar("Flipnote", *g_ressources->col_orange, *g_ressources->col_white);
    m_widgets = new WinWidgetContainer();

    m_widgets->AddWidget(new Label("Create new", g_reusableressources->font_ubuntumedium16, 0, -10, WidgetAllign_Center,[&](){ m_manager->SetState(new EditorState()); }));
    m_widgets->AddWidget(new Label("load", g_reusableressources->font_ubuntumedium16, 0, 10, WidgetAllign_Center,[&](){ printf("TODO\n"); }));
}

MainMenuState::~MainMenuState() {
    delete m_widgets;
    delete m_background;
    delete m_topbar;
}


void MainMenuState::Update() {
    m_topbar->Update();
    m_widgets->Update();
    m_background->Update();
}

void MainMenuState::Render() {
    m_background->Render();
    m_widgets->Render();
    m_topbar->Render();
}
