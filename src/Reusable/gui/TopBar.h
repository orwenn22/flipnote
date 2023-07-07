#ifndef TOPBAR_H
#define TOPBAR_H

#include <string>

struct SDL_Color;
struct SDL_Renderer;
struct SDL_Texture;

class TopBar {
    public:
    TopBar(std::string text, SDL_Color bg, SDL_Color fg);
    ~TopBar();

    void Update();
    void Render();

    void SetColor(SDL_Color bg, SDL_Color fg);
    void SetText(std::string text);

    private:
    void UpdateTextures();

    bool m_mouseonclosebutton;
    bool m_mouseonmaximize;
    bool m_mouseonbar;

    SDL_Texture* m_titletexttexturefg;
    SDL_Texture* m_titletexttexturebg;
    int m_titlewidth;
    int m_titleheight;

    float m_y;

    SDL_Color* m_bg;
    SDL_Color* m_fg;
    std::string m_text;
};

#endif