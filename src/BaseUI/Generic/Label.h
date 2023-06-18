#ifndef LABEL_H
#define LABEL_H

#include "ClickableWidget.h"

#include <string>

struct SDL_Texture;

class Label : public ClickableWidget {
    public:
    Label(WidgetContainer* container, int x, int y, std::string text, ClickabbleWidgetCallback callback);
    ~Label();

    void Render() override;

    private:
    SDL_Texture* m_texture;
};

#endif