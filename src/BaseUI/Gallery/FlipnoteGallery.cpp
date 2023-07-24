#include "FlipnoteGallery.h"

#include <filesystem>

#include "../../Reusable/gui/WinWidgetContainer.h"

FlipnoteGallery::FlipnoteGallery(std::string directory, State* parrentstate) {
    m_cwd = directory;
    m_state = parrentstate;
    m_widgets = new WinWidgetContainer();
    m_currentpageindex = 0;

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
    m_currentpageindex = 0;
    m_filenames.clear();

    if(m_cwd.empty()) {
        printf("FlipnoteGallery::ReloadFiles : cwd invalid\n");
        return;
    }

    printf("FlipnoteGallery::ReloadFiles : listing files :\n");
    for(auto entry : std::filesystem::directory_iterator(m_cwd)) {
        auto filename = entry.path().string();
        //TODO : check if the file content is valid
        if(filename.size() > 4 && filename.substr(filename.size()-4) == ".fnt") {
            printf("                             | %s\n", filename.c_str());
            m_filenames.push_back(filename);
        }
    }
}

// 0 -> 0
// 1 -> 1
//  ....
// 6 -> 1
// 7 -> 2
int FlipnoteGallery::GetPageCount() {
    return (m_filenames.size()-1)/6 + 1;
}
