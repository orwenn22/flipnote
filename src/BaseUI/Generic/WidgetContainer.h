#ifndef WIDGETCONTAINER_H
#define WIDGETCONTAINER_H

#include <vector>

class Widget;

class WidgetContainer {
    public:
    WidgetContainer();
    virtual ~WidgetContainer();

    

    //Add a widget to the container
    void AddWidget(Widget* widget);

    //Intended to be used by the container's widget
    virtual void GetAbsoluteSize(int* w, int* h) = 0;
    virtual int GetAbsoluteWidth() = 0;
    virtual int GetAbsoluteHeight() = 0;


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