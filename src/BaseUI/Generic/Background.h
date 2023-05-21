#ifndef BACKGROUND_H
#define BACKGROUND_H

struct SDL_Renderer;
struct SDL_Texture;

class Background {
    public:
    Background(SDL_Renderer* renderer);
    ~Background();

    void Update(SDL_Renderer* renderer);
    void Render(SDL_Renderer* renderer);

    private:

    void UpdateTexture(SDL_Renderer* renderer);

    SDL_Texture* m_bgtexture;
    int m_width;
    int m_height;

    int m_tilewidth;
    int m_tileheight;
};

#endif