#ifndef WIDGET_H
#define WIDGET_H

#include "../GraphicalObject.h"

class WidgetContainer;


enum WidgetAllign {
    WidgetAllign_None       = 0b00000000,

    WidgetAllign_HCenter    = 0b00010000,
    WidgetAllign_Left       = 0b00100000,

    WidgetAllign_VCenter    = 0b00000001,
    WidgetAllign_Bottom     = 0b00000010,

    WidgetAllign_Center     = WidgetAllign_HCenter | WidgetAllign_VCenter,
    WidgetAllign_BottomLeft = WidgetAllign_Left | WidgetAllign_Bottom,
};

class Widget : public GraphicalObject {
    public:
    Widget(
        int x, int y, int w = 0, int h = 0,
        WidgetAllign allignment = WidgetAllign_None
    );
    virtual ~Widget();

    //TODO : Already declared in GraphicalObject.h, might be a problem ?
    virtual void Update() = 0;
    virtual void Render() = 0;

    bool IsOvered();

    //Calculate and update the position of the widget using m_(x/y)offset and m_allignment.
    //Depending of m_allignment, the origin of the offset will be different.
    //if the widget have no container, it will be the same behavior as WidgetAllign_None
    void UpdatePos();

    //Get the x position of the widget in the container
    int GetX();
    //Get the y position of the widget in the container
    int GetY();
    //Get the position of the widget in the container
    void GetPos(int* x, int* y);

    //Change the offset position from the alligment origin
    void SetXOffset(int x);
    void SetYOffest(int y);


    //Get the x position of the widget in the window
    int GetAbsoluteX();
    //Get the y position of the widget in the window
    int GetAbsoluteY();
    //Get the position of the widget in the window
    void GetAbsolutePosition(int* x, int* y);

    //Called by WidgetContainer::AddWidget()
    void SetContainer(WidgetContainer* container);
    

    protected:

    virtual void OnContainerAdd();


    WidgetContainer* m_container;

    //Used for allignment (in UpdatePos)
    //Set with x and y by the constructor.
    int m_xoffset;
    int m_yoffset;
    WidgetAllign m_allignment;

    int m_w;
    int m_h;
    

    private:
    //X and Y are calculated by UpdatePos depending on m_allignment and shoud not be assigned manually.
    //They are privated so subclass can't rewrite them.
    //Can be accessed using GetX() and GetY();
    int m_x;
    int m_y;
};

#endif
