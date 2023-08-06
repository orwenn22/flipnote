#ifndef FLIPNOTEEDITOR_H
#define FLIPNOTEEDITOR_H

#include <string>

#include "../../Core/Flipnote/PaintCondition.h"

class EditorBrushButton;
class EditorPenButton;
class Flipnote;
class FlipnoteDisplay;
class FlipnoteFrame;
class FullscreenPopup;
class PopupMenu;
class State;
class WinWidgetContainer;

struct SDL_Surface;
struct SDL_Renderer;

class FlipnoteTimeline;


PaintCondition GetBrush(int index);
int GetBrushCount();

class FlipnoteEditor {
    public:
    //The FlipnoteEditor doesn't have ownership of fn, and must be deallocated manually AFTER destroying the FlipnoteEditor
    FlipnoteEditor(Flipnote* fn, std::string filename = "");
    ~FlipnoteEditor();

    void Update();
    void Render();


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
    FlipnoteDisplay* GetDisplay();


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

    void OpenFullscreenPopup(FullscreenPopup* popup);
    void CloseFullscreenPopup();
    bool IsFullscreenPopupOpen();

    void SetParentState(State* state);
    State* GetParentState();

    void SetFileName(std::string name);
    std::string GetFileName();
    void Save();
    void SaveAs();

    //True if the user is using a drawing tool
    //It is set to true by m_display->UpdateMouseInput
    bool m_isdrawing;

    //If set to true, it means the cached texture of current frame need to be reloaded.
    //This will be done when the user finish drawing
    bool m_needcachedtextureupdate;

    //If set to true, the brush will be inversed
    bool m_invertpaint;

    //Is set to true, the animation is playing;
    bool m_animmationplaying;

    //This the layer index selected by the user
    int m_targetlayer;

    private:

    //If m_isdrawing is set to true, this will get executed.
    //It's basicly the method that is in charge of painting on the current frame (both the fram itself and the display's texture).
    void UpdateDraw();

    void UpdateAnimation();

    void UpdatePopupMenu();
    void UpdateTimeline();

    void UpdateFullscreenPopup();

    void HandleEscapeKey();

    std::string m_filename;

    Flipnote* m_flipnote;

    //All ui layers (from bottom to top)
    FlipnoteDisplay* m_display;
    FlipnoteTimeline* m_timeline;
    WinWidgetContainer* m_editorbuttons;    //Contains all the buttons of the "overlay" (color, brush...)
    PopupMenu* m_popupmenu;
    FullscreenPopup* m_fullscreenpopup;

    bool m_fullscreenpopupneeddeletion;
    bool m_fullscreenpopupneedreplacement;
    FullscreenPopup* m_futurfullscreenpopup;

    State* m_parentstate;

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

    //For when the animation is playing (the time current frame have been displayed in second)
    float m_animationcooldown;
};

#endif
