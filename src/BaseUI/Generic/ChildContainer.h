#ifndef CHILDCONTAINER_H
#define CHILDCONTAINER_H

#include "WidgetContainer.h"
#include "Widget.h"

struct SDL_Texture;

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
    int GetAbsoluteWidth();
    int GetAbsoluteHeight();

    //Check if the mouse is overring the container (aka the widget).
    //Just use Widget::IsOvered
    bool IsMouseOvering();

    //Get the relative mouse position from the top left of the container/Widget.
    void GetRelativeMousePos(int* x, int* y);

    //is set to true, g_runtime->mouseused will be set to true if the mouse is overing the ChildContainer
    bool m_issolid;

    protected:
    SDL_Texture* m_texture; //content
    
    //WidgetContainer* m_parrent;
};

#endif