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
    SDL_Texture* CopyToTexture(int w, int h, bool transparent = false);

    //Copy all layers (from bottom to top) to SDL_Textures and returns them in a vector
    //All pixel with color of index 0 are transparent.
    std::vector<SDL_Texture*> CopyToTextures();


    //Overwrite the texture with all the layers of the frame stacked together
    //This won't allocate additional textures.
    //The texture passed as a parameter must be the correct size
    int OverwriteTexture(SDL_Texture* texture, bool transparent);

    //Overwrite the textures with the layers of the frame.
    //This won't allocate additional textures, it will use the existing ones.
    //They must be the correct size. All terxtures are cleared with treansparency
    int OverwriteTextures(std::vector<SDL_Texture*>& textures);
    

    ///////////////////////////////////
    // For editing

    //Set a pixel in one of the layers.
    void SetPixel(int x, int y, int layerindex, int colorindex);

    //Get the color of the first non-blank pixel at specific coordonates from the layers (from top to bottom)
    //Return 0 is the pixel is set 0 in all layers
    unsigned char GetPixel(int x, int y);
    
    //Get a pixel in one of the layer.
    unsigned char GetPixel(int x, int y, int layerindex);


    //This one is time consuming and should not be used whenever possible.
    //Instead, the cached texture should be manually updated by what is editing the frame
    void UpdateCachedTexture();

    //Returns m_cachedtexture
    SDL_Texture* GetCachedTexture();


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

    SDL_Texture* m_cachedtexture;
};


#endif
