#include "Utils.h"

#include <string>

#include "DeltaTime.h"
#include "Ressources.h"
#include "RunState.h"

void RenderDebugOverlay(int basex, int basey) {
    RenderText((std::string("Renderer : ")  + g_runstate->rendererinfo->name).c_str()       , basex, basey, g_reusableressources->font_ubuntumedium16, {0, 255, 0, 255}, {0, 0, 0, 255});
    RenderText((std::string("DeltaTime : ") + std::to_string(g_deltatime)).c_str()          , basex, basey+18, g_reusableressources->font_ubuntumedium16, {0, 255, 0, 255}, {0, 0, 0, 255});
    RenderText((std::string("FrameTime : ") + std::to_string(GetCurrentFrameTime())).c_str(), basex, basey+36, g_reusableressources->font_ubuntumedium16, {0, 255, 0, 255}, {0, 0, 0, 255});
    RenderText((std::string("FPS : ")       + std::to_string(1.0/g_deltatime)).c_str()       , basex, basey+54, g_reusableressources->font_ubuntumedium16, {0, 255, 0, 255}, {0, 0, 0, 255});
}
