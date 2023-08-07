#ifndef LABEL_H
#define LABEL_H

#include "ClickableWidget.h"

#include <string>

struct SDL_Color;
struct SDL_Texture;
struct _TTF_Font;

class Label : public ClickableWidget {
    public:
    Label(
        std::string text, _TTF_Font* font, SDL_Color color,
        int x, int y, WidgetAllign allignment = WidgetAllign_None,
        ClickabbleWidgetCallback callback = [](){}
    );
    ~Label();

    void Render() override;

    void SetText(std::string text);

    private:

    SDL_Texture* m_texture;

    _TTF_Font* m_font;
    SDL_Color* m_color;
};

#endif