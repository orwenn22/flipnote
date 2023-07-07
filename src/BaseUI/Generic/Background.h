#ifndef BACKGROUND_H
#define BACKGROUND_H

struct SDL_Renderer;
struct SDL_Texture;

class Background {
    public:
    Background();
    ~Background();

    void Update();
    void Render();

    private:

    void UpdateTexture();

    SDL_Texture* m_bgtexture;
    int m_width;
    int m_height;

    int m_tilewidth;
    int m_tileheight;
};

#endif