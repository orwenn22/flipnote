#ifndef LABEL_H
#define LABEL_H

#include "PopupMenuItem.h"

#include <string>

struct SDL_Texture;

class Label : public PopupMenuItem {
    public:
    Label(PopupMenu* popupmenu, int x, int y, std::string text);
    ~Label();

    void Update();
    void Render();

    private:
    SDL_Texture* m_texture;
};

#endif