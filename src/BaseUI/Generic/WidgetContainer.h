#ifndef WIDGETCONTAINER_H
#define WIDGETCONTAINER_H

#include <vector>
#include <stddef.h>

class Widget;

class WidgetContainer {
    public:
    WidgetContainer(WidgetContainer* parrent = NULL);
    virtual ~WidgetContainer();


    //Add a widget to the container
    void AddWidget(Widget* widget);

    //Get the position of the top of the container on the parrent container (if there is any)
    virtual int GetX() = 0;
    virtual int GetY() = 0;
    void GetPosition(int* x, int* y);

    //Get the position of the top left of the container on the window
    int GetAbsoluteX();
    int GetAbsoluteY();
    void GetAbsolutePosition(int* x, int* y);

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

    WidgetContainer* m_parrent;

    private:
    std::vector<Widget*> m_widgets;
};

#endif