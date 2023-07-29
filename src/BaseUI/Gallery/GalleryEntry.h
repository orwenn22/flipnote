#ifndef GALLERYENTRY_H
#define GALLERYENTRY_H

#include "../../Reusable/gui/ClickableWidget.h"

#include <string>

class FlipnoteGallery;
struct SDL_Color;
struct SDL_Texture;

class GalleryEntry: public ClickableWidget {
    public:
    GalleryEntry(FlipnoteGallery* gallery, int indexoffset, int x, int y, WidgetAllign allignment);
    ~GalleryEntry();

    void PreUpdate();
    void Render();

    private:
    void ReloadContent();

    void LoadPreview();
    void UnloadContent();

    FlipnoteGallery* m_gallery;
    int m_currentpage;
    int m_indexoffset;

    SDL_Texture* m_textname;

    std::string m_filepath;
    SDL_Texture* m_previewtexture;      //256 x 192
};

#endif
