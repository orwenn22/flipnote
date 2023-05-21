#include "PopupMenuTop.h"

#include "../../Core/Tileset.h"
#include "../../Core/Ressources.h"
#include "../../Core/RunState.h"
#include "../../Globals.h"
#include "PopupMenu.h"

static TilesetRectData topoverlaytiles = {
    0,   1,  2,
    6,   7,  8,
    18, 19, 20
};

PopupMenuTop::PopupMenuTop(PopupMenu* popupmenu, int width) : Widget(popupmenu) {
    m_w = width;
}

void PopupMenuTop::Update() {}

void PopupMenuTop::Render() {
    g_ressources->tileset_popupmenu->DrawRectangle(g_runstate->renderer, 0, 0, m_popupmenu->GetAbsoluteWidth(), m_w, &topoverlaytiles);
}
