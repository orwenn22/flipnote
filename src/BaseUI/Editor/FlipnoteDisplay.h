#ifndef FLIPNOTEDISPLAY_H
#define FLIPNOTEDISPLAY_H

#include <vector>

class FlipnoteEditor;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;

//This class display the current frame of the editor.
//RefreshTexture must be called each time the user switch the current frame.
class FlipnoteDisplay {
    public:
    FlipnoteDisplay(FlipnoteEditor* editor);
    ~FlipnoteDisplay();

    void Update();
    void Render();

    //Get the x and y index of the pixel being overred
    void GetMousePosPixel(int* x, int* y);

    //Get the x and y position of the mouse from the canvas's top left
    void GetMousePosRelative(int* x, int* y);

    //Fully reload the texture of the display (when the user switch frame for exemple)
    void RefreshTexture(SDL_Renderer* renderer);

    //Get a texture of the display.
    //Used by the editor in order to pass a texture to FlipnotePainter
    SDL_Texture* GetTexture(int layerindex);

    //True if the mouse is overring the display
    bool IsMouseOnDisplay();

    //Used to link m_showpreviousframepreview to a checkbox
    bool* GetShowPreviousFramePreviewPtr();

    private:
    //Check all the mouse interactions
    void UpdateMouseInput();

    //Check the mousewheel's movements and handle them.
    void HandleZoom();

    void RenderGrid();

    void RenderToolPreview();


    //The parrent editor of the display
    FlipnoteEditor* m_editor;

    //Copy of the current current frame being edited on the gpu side (contain all layers).
    //Reloaded by RefreshTexture (each time the user switch page).
    std::vector<SDL_Texture*> m_currentframetextures;

    SDL_Texture* m_toolpreview;

    SDL_Texture* m_previousframepreview;
    bool m_showpreviousframepreview;

    int m_x;
    int m_y;
    int m_unzoomedwidth;        //width without conciderring zoom (aka width of a frame of the flipnote)
    int m_unzoomedheight;       //height without conciderring zoom (aka height of a frame of the flipnote)
    float m_scale;

    bool m_followmouse;
    int m_mouseoffsetx;
    int m_mouseoffsety;

};

#endif
