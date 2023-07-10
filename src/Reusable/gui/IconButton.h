#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#include "ClickableWidget.h"

struct SDL_Texture;

class IconButton : public ClickableWidget {
    public:
    IconButton(
        SDL_Texture* texture,
        int x, int y, int w = 0, int h = 0,
        WidgetAllign allignment = WidgetAllign_None,
        ClickabbleWidgetCallback callback = []() -> void {}
    );

    virtual ~IconButton();

    void Render() override;

    private:
    SDL_Texture* m_texture;
};

#endif
