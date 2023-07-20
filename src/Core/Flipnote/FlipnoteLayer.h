#ifndef FLIPNOTELAYER_H
#define FLIPNOTELAYER_H

#include <cstdio>

class FlipnoteFrame;

struct SDL_Texture;


class FlipnoteLayer {
    public:
    FlipnoteLayer(FlipnoteFrame* frame, int w, int h);
    FlipnoteLayer(FlipnoteFrame* frame, int w, int h, FILE* infile);
    ~FlipnoteLayer();


    //Copy m_pixels to a SDL_Texture.
    SDL_Texture* CopyToTexture(bool transparentbackground);
    SDL_Texture* CopyToTexture(int w, int h, bool transparentbackground);   //w and h are the size of the created texture

    //Will copy the pixel data to the texture
    //Must be the same size of the frame
    int OverwriteTexture(SDL_Texture* texture, bool transparentbackground);


    //Set a pixel in m_pixels.
    void SetPixel(int x, int y, int colorindex);

    //Get a pixel in m_pixels.
    unsigned char GetPixel(int x, int y);

    void Save(FILE* file);


    //The parrent frame of the layer
    FlipnoteFrame* m_frame;


    private:
    //Array of values corresponding to a color in the palette (m_frame->m_flipnote->GetPalette())
    unsigned char* m_pixels;

    int m_width;
    int m_height;
};

#endif
