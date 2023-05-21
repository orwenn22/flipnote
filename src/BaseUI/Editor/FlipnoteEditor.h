#ifndef FLIPNOTEEDITOR_H
#define FLIPNOTEEDITOR_H

#include "../../Core/Flipnote/PaintCondition.h"

class EditorBrushButton;
class EditorPenButton;
class Flipnote;
class FlipnoteDisplay;
class FlipnoteFrame;
class PopupMenu;

struct SDL_Surface;
struct SDL_Renderer;



PaintCondition GetBrush(int index);
int GetBrushCount();

class FlipnoteEditor {
    public:
    FlipnoteEditor(SDL_Renderer* renderer, Flipnote* fn);
    ~FlipnoteEditor();

    void Update(SDL_Renderer* renderer);
    void Render(SDL_Renderer* renderer);

    //Load the page index in the surface, then call m_display->RefreshTexture
    void ChangePage(SDL_Renderer* renderer, int index);

    //Return a pointer to the current FlipnoteFrame
    FlipnoteFrame* CurrentFrame();

    //Get or change the current drawing color (m_currentcolorindex).
    int GetCurrentColorIndex();
    void SetCurrentColorIndex(int index);

    //Get or change the current brush (m_currentbrush)
    int GetCurrentBrush();
    void SetCurrentBrush(int index);

    int GetBrushSize();
    void SetBrushSize(int newsize);

    void OpenPopupMenu(PopupMenu* popupmenu);
    void ClosePopupMenu();

    //True if the user is using a drawing tool
    //It is set to true by m_display->UpdateMouseInput
    bool m_isdrawing;

    //If set to true, the brush will be inversed
    bool m_invertpaint;


    private:

    //If m_isdrawing is set to true, this will get executed.
    //It's basicly the method that is in charge of painting on the current frame (both the fram itself and the display's texture).
    void UpdateDraw(SDL_Renderer* renderer);

    Flipnote* m_flipnote;
    FlipnoteDisplay* m_display;
    PopupMenu* m_popupmenu;
    EditorPenButton* m_penbutton;
    EditorBrushButton* m_brushbutton;

    //The current page of the flipnote being edited
    int m_page;

    //The index of the color in the frame's palette.
    int m_currentcolorindex;

    //Used for line-drawing between each frames
    int m_previousx;
    int m_previousy;

    //The index of the current brush.
    int m_currentbrush;
    int m_brushsize;
};

#endif