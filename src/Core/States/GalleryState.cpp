#include "GalleryState.h"

#include <SDL.h>

#include "../../BaseUI/Gallery/FlipnoteGallery.h"
#include "../../Reusable/gui/Background.h"
#include "../../Reusable/gui/Label.h"
#include "../../Reusable/gui/TopBar.h"
#include "../../Reusable/State/StateManager.h"
#include "../../Reusable/Utils.h"
#include "../FlipnoteRessources.h"
#include "MainMenuState.h"


GalleryState::GalleryState() {
    std::string cwd = GetCWD();

    AddComponant(new Background(g_ressources->txtr_bgtile));

    AddComponant(
        new Label(
            "Back to main menu", g_ressources->font_ubuntumedium24, *g_ressources->col_green, 
            45, 34, WidgetAllign_None, 
            [&](){ m_manager->SetState(new MainMenuState());} 
        )
    );

    AddComponant(new Label("current path : " + cwd, g_ressources->font_ubuntumedium16, *g_ressources->col_green, 45, 60, WidgetAllign_None));

    AddComponant(new FlipnoteGallery(cwd, this));

    AddComponant(new TopBar("Flipnote gallery", *g_ressources->col_green, *g_ressources->col_white));
}

GalleryState::~GalleryState() {
    
}
