#ifndef FLIPNOTEFRAME_H
#define FLIPNOTEFRAME_H

struct SDL_Surface;
struct SDL_Texture;
struct SDL_Color;


class FlipnoteFrame {
    public:
    FlipnoteFrame(int w, int h);
    ~FlipnoteFrame();

    //Copy m_pixels to a SDL_Texture.
    SDL_Texture* CopyToTexture();
    SDL_Texture* CopyToTexture(int w, int h);
    

    void SetColor(int index, SDL_Color c);
    SDL_Color GetColor(int index);
    //NEVER MODIFY THE PALETTE RETURNED BY THIS
    SDL_Color* GetPalette();
    static SDL_Color GetDefaultColor(int index);
    //NEVER MODIFY THE PALETTE RETURNED BY THIS
    static SDL_Color* GetDefaultPalette();

    //Set a pixel in m_pixels.
    void SetPixel(int x, int y, int colorindex);

    //Get a pixel in m_pixels.
    unsigned char GetPixel(int x, int y);


    private:
    //SDL_Surface* m_surface;
    unsigned char* m_pixels;
    SDL_Color* m_colors;    //array of 8 elements

    int m_width;
    int m_height;
};


#endif
