#ifndef CHILDCONTAINER_H
#define CHILDCONTAINER_H

#include "WidgetContainer.h"
#include "Widget.h"

struct SDL_Texture;

//It's basicly a WidgetContainer inside a Widget
class ChildContainer : public WidgetContainer, public Widget {
    public:
    ChildContainer(
        WidgetContainer* parrent, 
        int x, int y, int w, int h, 
        WidgetAllign allignment = WidgetAllign_None
    );
    virtual ~ChildContainer();


    ////FROM Widget

    void Update();
    void Render();


    //// FROM WidgetContainer

    //Get the position of the top left of the container on the window (absolute position)
    int GetContainerX();
    int GetContainerY();


    //Intended to be used by the container's widget
    int GetContainerWidth();
    int GetContainerHeight();

    //Check if the mouse is overring the container (aka the widget).
    //Just use Widget::IsOvered
    bool IsMouseOvering();

    //Get the relative mouse position from the top left of the container/Widget.
    void GetRelativeMousePos(int* x, int* y);

    //if set to true, g_runtime->mouseused will be set to true if the mouse is overing the ChildContainer
    bool m_issolid;
    //if set to true, the outline of the container will be drawn
    bool m_drawoutline;

    protected:
    SDL_Texture* m_texture; //content
    
    //WidgetContainer* m_parrent;
};

#endif
