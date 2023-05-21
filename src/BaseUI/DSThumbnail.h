#ifndef THUMBNAIL_H
#define THUMBNAIL_H

struct SDL_Renderer;
struct SDL_Texture;

class DSThumbnail {
    public:
    DSThumbnail(SDL_Renderer* renderer, const char* filename);  //filename is a ppm file
    ~DSThumbnail();

    void Render(SDL_Renderer* renderer, float x, float y, float scale);

    private:
    SDL_Texture* m_thumbnailtexture;
};

#endif