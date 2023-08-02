#include "FlipnoteGallery.h"

#include <algorithm>
#include <filesystem>
#include <SDL3/SDL.h>

#include "../../Core/Flipnote/Flipnote.h"
#include "../../Core/FlipnoteRessources.h"
#include "../../Core/States/MainMenuState.h"
#include "../../Reusable/gui/WinWidgetContainer.h"
#include "../../Reusable/gui/Label.h"
#include "../../Reusable/RunState.h"
#include "../../Reusable/State/StateManager.h"
#include "../../Reusable/State/State.h"
#include "GalleryEntry.h"

FlipnoteGallery::FlipnoteGallery(std::string directory, State* parrentstate) {
    m_cwd = directory;
    m_state = parrentstate;
    m_widgets = new WinWidgetContainer();
    m_currentpageindex = 0;
    m_elementperpage = 6;


    ///////////////////////////////
    // Entry widgets

    //FIXME : 6 entry per page hardcoded
    m_widgets->AddWidget(new GalleryEntry(this, 0, -270, -108, WidgetAllign_Center));
    m_widgets->AddWidget(new GalleryEntry(this, 1, 0, -108, WidgetAllign_Center));
    m_widgets->AddWidget(new GalleryEntry(this, 2, 270,   -108, WidgetAllign_Center));
    m_widgets->AddWidget(new GalleryEntry(this, 3,  -270,   108, WidgetAllign_Center));
    m_widgets->AddWidget(new GalleryEntry(this, 4, 0,  108, WidgetAllign_Center));
    m_widgets->AddWidget(new GalleryEntry(this, 5,  270,  108, WidgetAllign_Center));

    m_pageinfo = new Label("Whatever", g_ressources->font_ubuntumedium16, *g_ressources->col_green, 0, -220, WidgetAllign_Center);
    m_widgets->AddWidget(m_pageinfo);

    ///////////////////////////////
    // Arrows to switch page

    m_widgets->AddWidget(
        new Label(
            "<-", g_ressources->font_ubuntumedium24, *g_ressources->col_green, 
            -450, 0, WidgetAllign_Center,
            [&]() {
                SetCurrentPage(GetCurrentPage()-1);
            }
        )
    );

    m_widgets->AddWidget(
        new Label(
            "->", g_ressources->font_ubuntumedium24, *g_ressources->col_green, 
            450, 0, WidgetAllign_Center,
            [&]() {
                SetCurrentPage(GetCurrentPage()+1);
            }
        )
    );


    ///////////////////////////////
    // Reload the files

    ReloadFiles();
}

FlipnoteGallery::~FlipnoteGallery() {
    delete m_widgets;
}

void FlipnoteGallery::Update() {
    m_widgets->Update();

    if(g_runstate->IsKeyPressed(SDLK_ESCAPE) && m_state != nullptr) {
        m_state->GetManager()->SetState(new MainMenuState);
    }
}
void FlipnoteGallery::Render() {
    m_widgets->Render();
}


void FlipnoteGallery::ReloadFiles() {
    m_filenames.clear();

    //check if cwd is not empty
    if(m_cwd.empty()) {
        printf("FlipnoteGallery::ReloadFiles : cwd invalid\n");
        return;
    }

    //Iterate through all the files of cwd
    printf("FlipnoteGallery::ReloadFiles : listing files :\n");
    for(auto entry : std::filesystem::directory_iterator(m_cwd)) {
        std::string filename = entry.path().string();
        //check if the file and its header is valid
        if(CheckFlipnoteFile(filename)) {
            printf("                             | %s\n", filename.c_str());
            m_filenames.push_back(filename);
        }
    }

    //sort in alphabetical order
    std::sort(m_filenames.begin(), m_filenames.end());

    //Set current page to 0 (first page)
    SetCurrentPage(0);
}


int FlipnoteGallery::GetFileCount() {
    return m_filenames.size();
}

std::string FlipnoteGallery::GetFileName(int index) {
    if(index < 0 || index >= GetFileCount()) {
        return "";
    }

    return m_filenames[index];
}

int FlipnoteGallery::GetPageCount() {
    //-1 because if there are ${m_elementperpage} elements then there is 1 page
    return (m_filenames.size()-1)/m_elementperpage + 1;
}

int FlipnoteGallery::GetCurrentPage() {
    return m_currentpageindex;
}

void FlipnoteGallery::SetCurrentPage(int index) {
    int page_count = GetPageCount();
    if(index < 0 || page_count == 0) index = 0;
    if(index >= page_count) index = page_count-1;

    m_currentpageindex = index;
    m_pageinfo->SetText(std::to_string(m_currentpageindex+1) + "/" + std::to_string(GetPageCount()));
}

int FlipnoteGallery::ElementPerPage() {
    return m_elementperpage;
}


State* FlipnoteGallery::ParrentState() {
    return m_state;
}
