#ifndef FLIPNOTEFRAME_H
#define FLIPNOTEFRAME_H

#include <cstdio>
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


    ///////////////////////////////////
    // Texture related things

    //Copy all layers stacked together to a SDL_Texture.
    //The background is the same color as the one at index 0 of the palette if transparent is set to false
    SDL_Texture* CopyToTexture(bool transparent = false);

    //Same as above, but with a width and height for the target texture
    //There is no transparency, the background will always be the color at index 0 of the palette
    SDL_Texture* CopyToTexture(int w, int h);

    //Copy all layers (from bottom to top) to SDL_Textures and returns them in a vector
    //All pixel with color of index 0 are transparent.
    std::vector<SDL_Texture*> CopyToTextures();
    

    ///////////////////////////////////
    // For editing

    //Set a pixel in one of the layers.
    void SetPixel(int x, int y, int layerindex, int colorindex);

    //Get the color of the first non-blank pixel at specific coordonates from the layers (from top to bottom)
    //Return 0 is the pixel is set 0 in all layers
    unsigned char GetPixel(int x, int y);
    
    //Get a pixel in one of the layer.
    unsigned char GetPixel(int x, int y, int layerindex);


    ///////////////////////////////////
    // Other

    //Save the frame to a file
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
