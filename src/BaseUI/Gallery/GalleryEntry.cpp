#include "GalleryEntry.h"

#include <SDL3/SDL.h>

#include "FlipnoteGallery.h"
#include "../../Core/Flipnote/Flipnote.h"
#include "../../Core/Flipnote/FlipnoteFrame.h"
#include "../../Core/FlipnoteRessources.h"
#include "../../Core/States/EditorState.h"
#include "../../Core/RoundCorner.h"
#include "../../Reusable/Utils.h"
#include "../../Reusable/RunState.h"
#include "../../Reusable/State/StateManager.h"
#include "../../Reusable/Tileset.h"


GalleryEntry::GalleryEntry(FlipnoteGallery* gallery, int indexoffset, int x, int y, WidgetAllign allignment) 
: ClickableWidget(x, y, 260, 196, allignment) {
    m_gallery = gallery;
    m_indexoffset = indexoffset;
    m_currentpage = -1;

    m_textname = nullptr;
    m_previewtexture = nullptr;
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
            m_gallery->ParrentState()->GetManager()->SetState(new EditorState(flipnote, m_filepath));
        }
        else {
            printf("   -> no path :(\n");
        }
    };
}

GalleryEntry::~GalleryEntry() {
    UnloadContent();
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
        //"blank" rectangle
        SDL_SetTextureColorMod(g_ressources->txtr_rounded_rect_line, 128, 128, 128);
        g_ressources->tileset_rounded_rect_line->DrawRectangle(g_runstate->renderer, &dest, g_ressources->rectdata_basic_rect);
        return;
    }

    //Render the rectangle
    SDL_Color c = *g_ressources->col_green;
    SDL_SetTextureColorMod(g_ressources->txtr_rounded_rect, c.r, c.g, c.b);
    g_ressources->tileset_rounded_rect->DrawRectangle(g_runstate->renderer, &dest, g_ressources->rectdata_basic_rect);

    //prepare to draw the texture
    dest.x += 2;
    dest.y += 2;
    dest.w -= 4;
    dest.h -= 4;

    //draw tbe preview itself
    SDL_RenderTexture(g_runstate->renderer, m_previewtexture, NULL, &dest);

    //Draw the rounded rectangle under the text
    dest.x += 12;
    dest.y += 180;
    dest.w = 230;
    dest.h = 28;
    g_ressources->tileset_rounded_rect->DrawRectangle(g_runstate->renderer, &dest, g_ressources->rectdata_basic_rect);

    //Draw the text itself
    int w,h;
    SDL_QueryTexture(m_textname, NULL, NULL, &w, &h);
    dest.x += 8;
    dest.y += 3;
    dest.w = (float)w;
    dest.h = (float)h;
    SDL_RenderTexture(g_runstate->renderer, m_textname, NULL, &dest);
}


void GalleryEntry::ReloadContent() {
    if(m_gallery == nullptr) return;
    
    m_currentpage = m_gallery->GetCurrentPage();

    UnloadContent();

    //Calculate the index of the button and get the corresponding file in the list
    int galleryindex = m_currentpage*m_gallery->ElementPerPage() + m_indexoffset;
    m_filepath = m_gallery->GetFileName(galleryindex);


    if(m_filepath.empty() == false) {   //if the path is empty that means the index is out of bounds
        m_textname = MakeTextTexture(RemovePath(m_filepath).c_str(), g_ressources->font_ubuntumedium16, *g_ressources->col_white);
        LoadPreview();
    }
}

void GalleryEntry::LoadPreview() {
    if(m_previewtexture != nullptr) return;

    //Init preview texture
    m_previewtexture = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 256, 192);
    SDL_SetTextureBlendMode(m_previewtexture, SDL_BLENDMODE_BLEND);

    //Load the first frame
    Flipnote* fn = Flipnote::Load(m_filepath.c_str(), true);

    //Draw on the preview's texture
    SDL_Texture* old_render_target = SDL_GetRenderTarget(g_runstate->renderer);
    SDL_SetRenderTarget(g_runstate->renderer, m_previewtexture);

    //Check if the flipnote loaded successfully
    if(fn != nullptr) {
        //Get the texture of the frame
        SDL_Color c = fn->GetColor(0);
        SDL_SetRenderDrawColor(g_runstate->renderer, c.r, c.g, c.b, 255);
        SDL_RenderClear(g_runstate->renderer);
        SDL_RenderTexture(g_runstate->renderer, fn->GetFrame(0)->GetCachedTexture(), NULL, NULL);

        //Deallocate the flipnote
        delete fn;
    }
    else {
        //Failed to load flipnote, fill the preview with red
        SDL_SetRenderDrawColor(g_runstate->renderer, 255, 0, 0, 255);
        SDL_RenderClear(g_runstate->renderer);
    }

    //Restore render target
    SDL_SetRenderTarget(g_runstate->renderer, old_render_target);

    //Apply rounded corners
    RoundTextureCorner(m_previewtexture);
}


void GalleryEntry::UnloadContent() {
    //Unload the text texture if necessary
    if(m_textname != nullptr) {
        SDL_DestroyTexture(m_textname);
        m_textname = nullptr;
    }

    //Unload the preview texture if necessary
    if(m_previewtexture != nullptr) {
        SDL_DestroyTexture(m_previewtexture);
        m_previewtexture = nullptr;
    }
}
