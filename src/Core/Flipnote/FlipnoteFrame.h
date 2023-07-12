#ifndef FLIPNOTEFRAME_H
#define FLIPNOTEFRAME_H

#include <bits/types/FILE.h>
#include <vector>

class Flipnote;
class FlipnoteLayer;

struct SDL_Surface;
struct SDL_Texture;
struct SDL_Color;


class FlipnoteFrame {
    public:
    FlipnoteFrame(Flipnote* flipnote, int w, int h);
    FlipnoteFrame(Flipnote* flipnote, int w, int h, FILE* infile);  //For loading from a file
    ~FlipnoteFrame();

    //Copy all layers stacked together to a SDL_Texture.
    SDL_Texture* CopyToTexture();
    SDL_Texture* CopyToTexture(int w, int h);
    //Copy all layers (from bottom to top) to SDL_Textures and returns them in a vector
    std::vector<SDL_Texture*> CopyToTextures();
    

    //Set a pixel in one of the layers.
    void SetPixel(int x, int y, int layerindex, int colorindex);
    //Get the color of the first non-blank pixel at specific coordonates from the layers (from top to bottom)
    unsigned char GetPixel(int x, int y);
    //Get a pixel in one of the layer.
    unsigned char GetPixel(int x, int y, int layerindex);

    void Save(FILE* file);

    //The parrent Flipnote of the frame
    //Public because it is accessed by FlipnotePainter
    Flipnote* m_flipnote;

    private:
    int m_width;
    int m_height;

    //m_layers[0] is the layer at the bottom
    std::vector<FlipnoteLayer*> m_layers;
};


#endif
