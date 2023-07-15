#ifndef LABEL_H
#define LABEL_H

#include "ClickableWidget.h"

#include <string>

struct SDL_Texture;
struct _TTF_Font;

class Label : public ClickableWidget {
    public:
    Label(
        std::string text, _TTF_Font* font,
        int x, int y, WidgetAllign allignment, 
        ClickabbleWidgetCallback callback = [](){}
    );
    ~Label();

    void Render() override;

    private:
    SDL_Texture* m_texture;
};

#endif