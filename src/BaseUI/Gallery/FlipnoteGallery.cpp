#include "FlipnoteGallery.h"

#include <algorithm>
#include <filesystem>
#include <SDL3/SDL.h>

#include "../../Core/Flipnote/Flipnote.h"
#include "../../Core/FlipnoteRessources.h"
#include "../../Reusable/gui/WinWidgetContainer.h"
#include "../../Reusable/gui/Label.h"
#include "GalleryEntry.h"

FlipnoteGallery::FlipnoteGallery(std::string directory, State* parrentstate) {
    m_cwd = directory;
    m_state = parrentstate;
    m_widgets = new WinWidgetContainer();
    m_currentpageindex = 0;
    m_elementperpage = 6;

    m_widgets->AddWidget(new GalleryEntry(this, 0, -130, -84, WidgetAllign_Center));
    m_widgets->AddWidget(new GalleryEntry(this, 1,  130, -84, WidgetAllign_Center));
    m_widgets->AddWidget(new GalleryEntry(this, 2, -130,   0, WidgetAllign_Center));
    m_widgets->AddWidget(new GalleryEntry(this, 3,  130,   0, WidgetAllign_Center));
    m_widgets->AddWidget(new GalleryEntry(this, 4, -130,  84, WidgetAllign_Center));
    m_widgets->AddWidget(new GalleryEntry(this, 5,  130,  84, WidgetAllign_Center));

    m_pageinfo = new Label("Page count : " + std::to_string(GetPageCount()), g_ressources->font_ubuntumedium16, *g_ressources->col_green, 0, -200, WidgetAllign_Center);
    m_widgets->AddWidget(m_pageinfo);

    m_widgets->AddWidget(
        new Label(
            "<-", g_ressources->font_ubuntumedium24, *g_ressources->col_green, 
            -300, 0, WidgetAllign_Center,
            [&]() {
                SetCurrentPage(GetCurrentPage()-1);
            }
        )
    );

    m_widgets->AddWidget(
        new Label(
            "->", g_ressources->font_ubuntumedium24, *g_ressources->col_green, 
            300, 0, WidgetAllign_Center,
            [&]() {
                SetCurrentPage(GetCurrentPage()+1);
            }
        )
    );

    ReloadFiles();
}

FlipnoteGallery::~FlipnoteGallery() {
    delete m_widgets;
}

void FlipnoteGallery::Update() {
    m_widgets->Update();
}
void FlipnoteGallery::Render() {
    m_widgets->Render();
}


void FlipnoteGallery::ReloadFiles() {
    m_filenames.clear();

    if(m_cwd.empty()) {
        printf("FlipnoteGallery::ReloadFiles : cwd invalid\n");
        return;
    }

    printf("FlipnoteGallery::ReloadFiles : listing files :\n");
    for(auto entry : std::filesystem::directory_iterator(m_cwd)) {
        std::string filename = entry.path().string();
        //TODO : check if the file content is valid
        if(CheckFlipnoteFile(filename)) {
            printf("                             | %s\n", filename.c_str());
            m_filenames.push_back(filename);
        }
    }

    std::sort(m_filenames.begin(), m_filenames.end());

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

// 0 -> 0
// 1 -> 1
//  ....
// 6 -> 1
// 7 -> 2
int FlipnoteGallery::GetPageCount() {
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
