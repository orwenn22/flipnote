#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "../GraphicalObject.h"

struct SDL_Renderer;
struct SDL_Texture;

//This provides a simple way to add a tiled background.
class Background : public GraphicalObject {
    public:
    Background(SDL_Texture* tiletexture);
    ~Background();

    void Update();
    void Render();

    private:

    void UpdateTexture();

    SDL_Texture* m_bgtexture;
    SDL_Texture* m_tiletexture;
    int m_width;
    int m_height;

    int m_tilewidth;
    int m_tileheight;
};

#endif