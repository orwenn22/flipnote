#ifndef FLIPNOTE_TEXTFIELD_H
#define FLIPNOTE_TEXTFIELD_H

#include "Widget.h"

#include <string>

struct SDL_Texture;


class TextField : public Widget {
public:
    TextField(
        std::string* target,
        int x, int y, int w = 0,
        WidgetAllign allignment = WidgetAllign_None
    );
    ~TextField();

    void Update() override;
    void Render() override;

private:
    std::string* m_target;
    SDL_Texture* m_texttexture;

    bool m_selected;

    void CheckForInput();
};


#endif //FLIPNOTE_TEXTFIELD_H
