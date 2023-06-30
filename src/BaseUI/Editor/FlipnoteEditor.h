#ifndef FLIPNOTEEDITOR_H
#define FLIPNOTEEDITOR_H

#include "../../Core/Flipnote/PaintCondition.h"

class EditorBrushButton;
class EditorPenButton;
class Flipnote;
class FlipnoteDisplay;
class FlipnoteFrame;
class PopupMenu;
class WinWidgetContainer;

struct SDL_Surface;
struct SDL_Renderer;

class FlipnoteTimeline;


PaintCondition GetBrush(int index);
int GetBrushCount();

class FlipnoteEditor {
    public:
    //The FlipnoteEditor doesn't have ownership of fn, and must be deallocated manually AFTER destroying the FlipnoteEditor
    FlipnoteEditor(SDL_Renderer* renderer, Flipnote* fn);
    ~FlipnoteEditor();

    void Update(SDL_Renderer* renderer);
    void Render(SDL_Renderer* renderer);


    //Return a pointer to the current FlipnoteFrame
    FlipnoteFrame* CurrentFrame();

    //Will set the frame that is currently being edited, and update the FlipnoteDisplay 
    //SHOULD BE THE ONLY WAY TO ACCESS AND MODIFY m_page
    void SetCurrentFrame(int index);
    int GetCurrentFrame();
    void GoToNextFrame();
    void GoToPreviousFrame();

    //Return a pointer to the flipnote being edited
    Flipnote* GetFlipnote();


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

    void OpenTimeline();
    void CloseTimeline();
    bool IsTimelineOpen();

    //True if the user is using a drawing tool
    //It is set to true by m_display->UpdateMouseInput
    bool m_isdrawing;

    //If set to true, the brush will be inversed
    bool m_invertpaint;

    //Is set to true, the animation is playing;
    bool m_animmationplaying;
    //Animation speed (lower is faster)
    int m_animationinterval;

    private:

    //If m_isdrawing is set to true, this will get executed.
    //It's basicly the method that is in charge of painting on the current frame (both the fram itself and the display's texture).
    void UpdateDraw(SDL_Renderer* renderer);

    void UpdateAnimation();

    void UpdatePopupMenu();
    void UpdateTimeline();

    Flipnote* m_flipnote;
    FlipnoteDisplay* m_display;
    PopupMenu* m_popupmenu;
    
    //Contains all the buttons of the "overlay" (color, brush...)
    WinWidgetContainer* m_editorbuttons;
    
    FlipnoteTimeline* m_timeline;

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

    //For when the animation is playing
    int m_animationcooldown;
};

#endif
