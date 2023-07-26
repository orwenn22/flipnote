#include "GalleryEntry.h"

#include <SDL3/SDL.h>

#include "FlipnoteGallery.h"
#include "../../Core/FlipnoteRessources.h"
#include "../../Reusable/Utils.h"
#include "../../Reusable/RunState.h"


GalleryEntry::GalleryEntry(FlipnoteGallery* gallery, int indexoffset, int x, int y, WidgetAllign allignment) 
: ClickableWidget(x, y, 250, 75, allignment) {
    m_gallery = gallery;
    m_indexoffset = indexoffset;
    m_currentpage = -1;

    m_textname = nullptr;
    m_filepath = "";


    m_callback = [&]() {
        printf("clicked on %i\n", m_indexoffset);
        if(m_filepath.empty() == false) {
            printf("-> path : %s\n", m_filepath.c_str());
        }
        else {
            printf("-> no path :(\n");
        }
    };
}

GalleryEntry::~GalleryEntry() {
    if(m_textname != nullptr) {
        SDL_DestroyTexture(m_textname);
        m_textname = nullptr;
    }
}

void GalleryEntry::PreUpdate() {
    if(m_gallery == nullptr) return;

    if(m_currentpage != m_gallery->GetCurrentPage()) {
        ReloadContent();
    }
}

void GalleryEntry::Render() {
    SDL_FRect dest = {(float)GetX(), (float)GetY(), (float)m_w, (float)m_h};

    if(m_textname == nullptr) {
        SDL_SetRenderDrawColor(g_runstate->renderer, 128, 128, 128, 255);
        SDL_RenderRect(g_runstate->renderer, &dest);
        return;
    }

    SDL_Color c = *g_ressources->col_green;

    SDL_SetRenderDrawColor(g_runstate->renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(g_runstate->renderer, &dest);

    dest.x += 5.0f;
    dest.y += 5.0f;

    int w,h;
    SDL_QueryTexture(m_textname, NULL, NULL, &w, &h);
    dest.w = w;
    dest.h = h;

    SDL_RenderTexture(g_runstate->renderer, m_textname, NULL, &dest);
}


void GalleryEntry::ReloadContent() {
    if(m_gallery == nullptr) return;
    
    m_currentpage = m_gallery->GetCurrentPage();

    if(m_textname != nullptr) {
        SDL_DestroyTexture(m_textname);
        m_textname = nullptr;
    }

    int galleryindex = m_currentpage*m_gallery->ElementPerPage() + m_indexoffset;
    m_filepath = m_gallery->GetFileName(galleryindex);

    if(m_filepath.empty() == false) {
        m_textname = MakeTextTexture(RemovePath(m_filepath).c_str(), g_ressources->font_ubuntumedium16, *g_ressources->col_white);
    }
}
