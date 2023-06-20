#ifndef FLIPNOTETIMELINE_H
#define FLIPNOTETIMELINE_H

#include <vector>

class ChildContainer;
class FlipnoteEditor;
class WidgetContainer;

struct SDL_FRect;
struct SDL_Texture;

class FlipnoteTimeline {
    public:
    FlipnoteTimeline(FlipnoteEditor* editor);
    ~FlipnoteTimeline();

    void Update();
    void Render();

    bool IsMouseOvering();

    //Action for buttons
    void AddFrame();
    void DeleteFrame();

    private:
    //Sliding animation from the bottom of the window
    void UpdateEnterAnimation();
    
    //Check for clicks on the frames.
    void UpdateFrames();
    //Draw the frames on the timeline.
    void RenderFrames();
    //Load the frame into a texture if it is not already done, then render it.
    void LoadAndRenderFrame(int index, SDL_FRect* dest);

    //Absolute Y position calculated by UpdateEnterAnimation()
    int m_y;

    //Used for the enter animation
    int m_yfrombottom;
    int m_yfrombottomdest;

    //Size of the size of a tile of the timeline (background)
    int m_tilewidth;
    int m_tileheight;

    //Scroll position
    int m_framesx;
    int m_framesxvelocity;


    FlipnoteEditor* m_editor;

    std::vector<SDL_Texture*> m_framestextures;

    ChildContainer* m_widgets;
};

#endif
