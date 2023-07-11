#ifndef FLIPNOTEFRAME_H
#define FLIPNOTEFRAME_H

#include <bits/types/FILE.h>

class Flipnote;

struct SDL_Surface;
struct SDL_Texture;
struct SDL_Color;


class FlipnoteFrame {
    public:
    FlipnoteFrame(Flipnote* flipnote, int w, int h);
    FlipnoteFrame(Flipnote* flipnote, int w, int h, FILE* infile);  //For loading from a file
    ~FlipnoteFrame();

    //Copy m_pixels to a SDL_Texture.
    SDL_Texture* CopyToTexture();
    SDL_Texture* CopyToTexture(int w, int h);
    

    //Set a pixel in m_pixels.
    void SetPixel(int x, int y, int colorindex);

    //Get a pixel in m_pixels.
    unsigned char GetPixel(int x, int y);

    void Save(FILE* file);

    //Public because it is accessed by FlipnotePainter
    Flipnote* m_flipnote;

    private:
    unsigned char* m_pixels;

    int m_width;
    int m_height;
};


#endif
