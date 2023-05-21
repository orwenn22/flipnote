#ifndef FLIPNOTEPAINTER_H
#define FLIPNOTEPAINTER_H

class FlipnoteFrame;
struct SDL_Texture;

#include "PaintCondition.h"

bool DefaultPaintCondition(int x, int y);

//This class is used in order to paint on the texture and on the frame at the same time.
class FlipnotePainter {
    public:
    FlipnotePainter(FlipnoteFrame* frame, SDL_Texture* texture, PaintCondition paintcondition = DefaultPaintCondition, bool invertpaint = false);

    //All of these will call PrepareRender
    void SetPixel(int x, int y, int colorindex);
    void DrawLine(int x0, int y0, int x1, int y1, int colorindex);
    void DrawLineEx(int x0, int y0, int x1, int y1, int thickness, int colorindex);
    void DrawCircle(int x, int y, int r, int colorindex);
    void FillCircle(int x, int y, int r, int colorindex);

    PaintCondition m_paintcondition;
    bool m_invertpaint;

    FlipnoteFrame* m_frame;
    SDL_Texture* m_texture;

    private:
    //All of these WON'T call PrepareRender
    void InternalSetPixel(int x, int y, int colorindex);
    void InternalDrawCircle(int x, int y, int r, int colorindex);
    void InternalFillCircle(int x, int y, int r, int colorindex);

    //Will set the renderer's draw color. Called at the beginning of each public draw function.
    //Returns the render target taht was before switching to m_texture.
    SDL_Texture* PrepareRender(int colorindex);

};

#endif