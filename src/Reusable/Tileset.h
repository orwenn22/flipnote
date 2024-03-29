#ifndef TILESET_H
#define TILESET_H

struct SDL_FRect;
struct SDL_Renderer;
struct SDL_Texture;

struct TilesetRectData {
    int topleftindex,
        topindex,
        toprightindex,
        leftindex,
        centerindex,
        rightindex,
        bottomleftindex,
        bottomindex,
        bottomrightindex;
};

class Tileset {
    public:
    Tileset(SDL_Texture* texture, int tilewidth, int tileheight);
    ~Tileset(); //Does't deallocate m_texture

    int XYToIndex(int tilex, int tiley);

    //Draw a single tile of the tileset
    //Use these if the tile needs to be at a different size on the target
    void DrawTile(SDL_Renderer* renderer, int tilenum, SDL_FRect* dest);
    void DrawTile(SDL_Renderer* renderer, int tilex, int tiley, SDL_FRect* dest);
    //Use these if the tile have the same size on the target
    void DrawTile(SDL_Renderer* renderer, int tilenum, int destx, int desty);
    void DrawTile(SDL_Renderer* renderer, int tilex, int tiley, int destx, int desty);

    void DrawRectangle(SDL_Renderer* renderer, int topleftx, int toplefty, int bottomrightx, int bottomrighty, TilesetRectData* rectdata);
    void DrawRectangle(SDL_Renderer* renderer, SDL_FRect* rect, TilesetRectData* rectdata);

    private:
    SDL_Texture* m_texture;
    int m_tilewidth;
    int m_tileheight;

    int m_tilecountx;
    int m_tilecounty;
    int m_tilecount;
};

#endif
