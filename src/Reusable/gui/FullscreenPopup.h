#ifndef FLIPNOTE_FULLSCREENPOPUP_H
#define FLIPNOTE_FULLSCREENPOPUP_H

#include "../GraphicalObject.h"
#include "WidgetContainer.h"

struct SDL_Texture;

class FullscreenPopup : public WidgetContainer, public GraphicalObject {
public:
    FullscreenPopup(int w, int h);
    virtual ~FullscreenPopup();

    void Update() override;
    void Render() override;


    int GetContainerX() override;
    int GetContainerY() override;
    int GetContainerWidth() override;
    int GetContainerHeight() override;

    bool IsMouseOvering() override;
    void GetRelativeMousePos(int* x, int* y) override;

private:
    SDL_Texture* m_texture;
    int m_width;
    int m_height;
};


#endif //FLIPNOTE_FULLSCREENPOPUP_H
