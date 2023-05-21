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
    

    void SetColor(int index, SDL_Color c);
    SDL_Color GetColor(int index);
    static SDL_Color GetDefaultColor(int index);

    //Set a pixel in m_pixels.
    void SetPixel(int x, int y, int colorindex);


    private:
    //SDL_Surface* m_surface;
    unsigned char* m_pixels;
    SDL_Color* m_colors;    //array of 8 elements

    int m_width;
    int m_height;
};

/////////////////////

/*
typedef void (*SetPixelFuction)(FlipnoteFrame*, int, int , int);

void FrameSetPixel(FlipnoteFrame* frame, int x, int y, int colorindex);
void FrameDrawLine(FlipnoteFrame* frame, int x0, int y0, int x1, int y1, int colorindex, SetPixelFuction setpixel);
void FrameDrawLineEx(FlipnoteFrame* frame, int x0, int y0, int x1, int y1, int thickness, int colorindex, SetPixelFuction setpixel);
void FrameDrawCircle(FlipnoteFrame* frame, int x, int y, int r, int colorindex, SetPixelFuction setpixel);
void FrameFillCircle(FlipnoteFrame* frame, int x, int y, int r, int colorindex,SetPixelFuction setpixel);
*/

#endif