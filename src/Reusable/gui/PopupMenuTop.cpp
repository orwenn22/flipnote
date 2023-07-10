#include "PopupMenuTop.h"

#include "../Ressources.h"
#include "../RunState.h"
#include "../Tileset.h"
#include "PopupMenu.h"

static TilesetRectData topoverlaytiles = {
    0,   1,  2,
    6,   7,  8,
    18, 19, 20
};

PopupMenuTop::PopupMenuTop(int width) : Widget(0, 0) {
    m_w = width;
}

void PopupMenuTop::Update() {}

void PopupMenuTop::Render() {
    g_reusableressources->tileset_popupmenu->DrawRectangle(g_runstate->renderer, 0, 0, m_container->GetContainerWidth(), m_w, &topoverlaytiles);
}

