#ifndef FLIPNOTEPAINTER_H
#define FLIPNOTEPAINTER_H

class FlipnoteFrame;
struct SDL_Color;
struct SDL_Texture;

#include "PaintCondition.h"

bool DefaultPaintCondition(int x, int y);

//This class is used in order to paint on the texture and on the frame at the same time.
class FlipnotePainter {
    public:
    //For drawing on a texture only (use Flipnote::GetDefaultPalette() as palette)
    FlipnotePainter(SDL_Texture* texture, PaintCondition paintcondition = DefaultPaintCondition, bool invertpaint = false);
    //For drawing on a texture and a frame at the same time (use frame->m_flipnote->Getpalette() as palette)
    FlipnotePainter(FlipnoteFrame* frame, int targetlayer, SDL_Texture* texture, PaintCondition paintcondition = DefaultPaintCondition, bool invertpaint = false);
    //For drawing on a texture only (use palette as palette)
    FlipnotePainter(SDL_Color* palette, SDL_Texture* texture, PaintCondition paintcondition = DefaultPaintCondition, bool invertpaint = false);

    //All of these will call PrepareRender
    void SetPixel(int x, int y, int colorindex);
    void DrawLine(int x0, int y0, int x1, int y1, int colorindex);
    void DrawLineEx(int x0, int y0, int x1, int y1, int thickness, int colorindex);
    void DrawCircle(int x, int y, int r, int colorindex);
    void FillCircle(int x, int y, int r, int colorindex);

    PaintCondition m_paintcondition;
    bool m_invertpaint;
    int m_targetlayer;

    FlipnoteFrame* m_frame;
    SDL_Texture* m_texture;
    SDL_Color* m_palette;

    private:
    //All of these WON'T call PrepareRender
    void InternalSetPixel(int x, int y, int colorindex);
    void InternalDrawCircle(int centerx, int centery, int r, int colorindex);
    void InternalFillCircle(int x, int y, int r, int colorindex);

    //Will set the renderer's draw color. Called at the beginning of each public draw function.
    //Returns the render target taht was before switching to m_texture.
    SDL_Texture* PrepareRender(int colorindex);

};

#endif
