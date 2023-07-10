#ifndef LABEL_H
#define LABEL_H

#include "ClickableWidget.h"

#include <string>

struct SDL_Texture;

class Label : public ClickableWidget {
    public:
    Label(
        std::string text,
        int x, int y, WidgetAllign allignment, 
        ClickabbleWidgetCallback callback = [](){}
    );
    ~Label();

    void Render() override;

    private:
    SDL_Texture* m_texture;
};

#endif