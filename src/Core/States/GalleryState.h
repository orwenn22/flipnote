#ifndef GALLERYSTATE_H
#define GALLERYSTATE_H

#include "../../Reusable/State/State.h"

#include <string>

class Background;
class TopBar;
class Label;

class GalleryState : public State {
    public:
    GalleryState();
    ~GalleryState();

    void Update();
    void Render();

    private:
    Background* m_background;
    TopBar* m_topbar;
    Label* m_pathtext;
    Label* m_backtext;

    std::string m_cwd;
};

#endif
