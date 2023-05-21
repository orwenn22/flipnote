#ifndef LABEL_H
#define LABEL_H

#include "Widget.h"

#include <string>

struct SDL_Texture;

class Label : public Widget {
    public:
    Label(PopupMenu* popupmenu, int x, int y, std::string text);
    ~Label();

    void Update();
    void Render();

    private:
    SDL_Texture* m_texture;
};

#endif