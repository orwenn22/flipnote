#ifndef GALLERYSTATE_H
#define GALLERYSTATE_H

#include "../../Reusable/State/SimpleState.h"

#include <string>

class Background;
class TopBar;
class Label;

class GalleryState : public SimpleState {
    public:
    GalleryState();
    ~GalleryState();
};

#endif
