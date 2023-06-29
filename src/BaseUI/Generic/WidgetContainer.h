#ifndef WIDGETCONTAINER_H
#define WIDGETCONTAINER_H

#include <vector>
#include <stddef.h>

class Widget;

class WidgetContainer {
    public:
    WidgetContainer();
    virtual ~WidgetContainer();


    //Add a widget to the container
    void AddWidget(Widget* widget);
    //Remove a widget from the container
    void RemoveWidget(Widget* widget);
    //Remove all Widget
    void Clear();


    //Get the position of the top left of the container on the window
    virtual int GetContainerX() = 0;
    virtual int GetContainerY() = 0;

    //Intended to be used by the container's widgets
    virtual int GetContainerWidth() = 0;
    virtual int GetContainerHeight() = 0;
    void GetAbsoluteSize(int* w, int* h);


    //Check if the mouse is overring the container.
    virtual bool IsMouseOvering() = 0;

    //Get the relative mouse position from the top left of the container.
    virtual void GetRelativeMousePos(int* x, int* y) = 0;


    protected:      //need to be used in subclasses
    void UpdateWidgets();
    void RenderWidgets();

    private:
    std::vector<Widget*> m_widgets;
};

#endif