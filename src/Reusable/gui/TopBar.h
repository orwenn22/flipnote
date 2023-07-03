#ifndef TOPBAR_H
#define TOPBAR_H

struct SDL_Renderer;
struct SDL_Texture;

class TopBar {
    public:
    TopBar(SDL_Renderer* renderer);
    ~TopBar();

    void Update();
    void Render(SDL_Renderer* renderer);

    private:
    bool m_mouseonclosebutton;
    bool m_mouseonmaximize;
    bool m_mouseonbar;

    SDL_Texture* m_titletexttexturewhite;
    SDL_Texture* m_titletexttextureorange;
    int m_titlewidth;
    int m_titleheight;

    float m_y;
};

#endif