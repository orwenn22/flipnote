#include "GalleryEntry.h"

#include <SDL3/SDL.h>

#include "FlipnoteGallery.h"
#include "../../Core/Flipnote/Flipnote.h"
#include "../../Core/FlipnoteRessources.h"
#include "../../Core/States/EditorState.h"
#include "../../Reusable/Utils.h"
#include "../../Reusable/RunState.h"
#include "../../Reusable/State/State.h"
#include "../../Reusable/State/StateManager.h"


GalleryEntry::GalleryEntry(FlipnoteGallery* gallery, int indexoffset, int x, int y, WidgetAllign allignment) 
: ClickableWidget(x, y, 250, 75, allignment) {
    m_gallery = gallery;
    m_indexoffset = indexoffset;
    m_currentpage = -1;

    m_textname = nullptr;
    m_filepath = "";


    m_callback = [&]() {
        printf("GalleryEntry callback : clicked on %i\n", m_indexoffset);

        //Check if there is a path loaded in the entry
        if(m_filepath.empty() == false) {
            printf("   -> path : %s\n", m_filepath.c_str());

            //Check if the entry is pointing to a gallery (we can't change the state if it's not)
            if(m_gallery == nullptr) return;

            //Load the flipnote from the path
            Flipnote* flipnote = Flipnote::Load(m_filepath.c_str());
            if(flipnote == nullptr) return;     //check for fail

            //Put it in a new state and replace the gallery by this state
            m_gallery->ParrentState()->GetManager()->SetState(new EditorState(flipnote));
        }
        else {
            printf("   -> no path :(\n");
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

    //check if the page was changed
    if(m_currentpage != m_gallery->GetCurrentPage()) {
        ReloadContent();
    }
}

void GalleryEntry::Render() {
    SDL_FRect dest = {(float)GetX(), (float)GetY(), (float)m_w, (float)m_h};

    //If the m_textname is not loaded, don't render the button (only the outline)
    if(m_textname == nullptr) {
        SDL_SetRenderDrawColor(g_runstate->renderer, 128, 128, 128, 255);
        SDL_RenderRect(g_runstate->renderer, &dest);
        return;
    }

    //Render the rectangle
    SDL_Color c = *g_ressources->col_green;
    SDL_SetRenderDrawColor(g_runstate->renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(g_runstate->renderer, &dest);

    //Reajust the dest rect for the text
    dest.x += 5.0f;
    dest.y += 5.0f;
    int w,h;
    SDL_QueryTexture(m_textname, NULL, NULL, &w, &h);
    dest.w = w;
    dest.h = h;

    //Render the text
    SDL_RenderTexture(g_runstate->renderer, m_textname, NULL, &dest);
}


void GalleryEntry::ReloadContent() {
    if(m_gallery == nullptr) return;
    
    m_currentpage = m_gallery->GetCurrentPage();

    //Unload the text texture if necessary
    if(m_textname != nullptr) {
        SDL_DestroyTexture(m_textname);
        m_textname = nullptr;
    }

    //Calculate the index of the button and get the corresponding file in the list
    int galleryindex = m_currentpage*m_gallery->ElementPerPage() + m_indexoffset;
    m_filepath = m_gallery->GetFileName(galleryindex);


    if(m_filepath.empty() == false) {   //if the path is empty that means the index is out of bounds
        m_textname = MakeTextTexture(RemovePath(m_filepath).c_str(), g_ressources->font_ubuntumedium16, *g_ressources->col_white);
    }
}
