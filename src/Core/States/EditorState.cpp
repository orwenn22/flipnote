#include "EditorState.h"

#include "../../BaseUI/Editor/FlipnoteEditor.h"
#include "../../BaseUI/Generic/Background.h"
#include "../../Reusable/gui/TopBar.h"
#include "../Flipnote/Flipnote.h"
#include "../Globals.h"
#include "../FlipnoteRessources.h"

#include <SDL.h>


EditorState::EditorState(Flipnote* flipnote) : State() {
    m_background = new Background();
    m_topbar = new TopBar("Flipnote Editor", *g_ressources->col_orange, *g_ressources->col_white);

    m_flipnote = flipnote;
    if(flipnote == nullptr) {   //if no flipnote is passed to the constructior
        printf("EditorState::EditorState : flipnote is nullptr, creating a new one\n");
        m_flipnote = new Flipnote(512, 384);
    }

    m_editor = new FlipnoteEditor(m_flipnote);
}

EditorState::~EditorState() {
    delete m_editor;
    printf("EditorState::~EditorState : destroyed FlipnoteEditor\n");
    delete m_flipnote;
    printf("EditorState::~EditorState : destroyed Flipnote\n");

    delete m_background;
    delete m_topbar;
}


void EditorState::Update() {
    m_topbar->Update();
    m_editor->Update();
    m_background->Update();
}

void EditorState::Render() {
    m_background->Render();
    m_editor->Render();
    m_topbar->Render();
}
