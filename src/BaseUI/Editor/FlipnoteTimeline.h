#ifndef FLIPNOTETIMELINE_H
#define FLIPNOTETIMELINE_H

#include <vector>

class ChildContainer;
class FlipnoteEditor;
class WidgetContainer;

struct SDL_Texture;

class FlipnoteTimeline {
    public:
    FlipnoteTimeline(FlipnoteEditor* editor);
    ~FlipnoteTimeline();

    void Update();
    void Render();

    bool IsMouseOvering();

    void AddFrame();

    private:
    void UpdateEnterAnimation();
    
    void UpdateFrames();
    void RenderFrames();

    int m_y;

    int m_yfrombottom;
    int m_yfrombottomdest;

    int m_tilewidth;
    int m_tileheight;


    int m_framesx;


    FlipnoteEditor* m_editor;

    std::vector<SDL_Texture*> m_framestextures;

    ChildContainer* m_widgets;
};

#endif
