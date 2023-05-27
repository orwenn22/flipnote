#include "EditorButton.h"

#include "../../Core/RunState.h"
#include "../../Globals.h"

#include <SDL.h>

EditorButton::EditorButton(FlipnoteEditor* editor, int x, int y, int w, int h, ButtonAllign allignment, EditorButtonCallback callback) {
    m_editor = editor;
    m_allignment = allignment;
    m_callback = callback;

    m_xoffset = x;
    m_yoffset = y;
    m_w = w;
    m_h = h;

    UpdatePos();
}

EditorButton::~EditorButton() {
    
}

void EditorButton::Update() {
    UpdatePos();

    if(g_runstate->mouseused) return;

    if(g_runstate->mousex >= m_x && g_runstate->mousex < m_x+m_w
    && g_runstate->mousey >= m_y && g_runstate->mousey < m_y+m_h) {
        g_runstate->mouseused = true;
    
        if(g_runstate->leftclick) {
            m_callback(this);
        }
    }
}

void EditorButton::Render() {
    SDL_FRect dest = {(float)m_x, (float)m_y, (float)m_w, (float)m_h};
    SDL_SetRenderDrawColor(g_runstate->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(g_runstate->renderer, &dest);
}


int EditorButton::GetX() {
    return m_x;
}
int EditorButton::GetY() {
    return m_y;
}


void EditorButton::UpdatePos() {
    if(m_allignment == ButtonAllign::ButtonAllign_None) {
        m_x = m_xoffset;
        m_y = m_yoffset;
        return;
    }

    if((m_allignment & ButtonAllign::ButtonAllign_Left) != 0) {
        m_x = g_runstate->winwidth - m_xoffset;
    }
    else {
        m_x = m_xoffset;
    }

    if((m_allignment & ButtonAllign::ButtonAllign_Bottom) != 0) {
        m_y = g_runstate->winheight - m_yoffset;
    }
    else {
        m_y = m_yoffset;
    }
}