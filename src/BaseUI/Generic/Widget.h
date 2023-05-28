#ifndef WIDGET_H
#define WIDGET_H

class WidgetContainer;


enum WidgetAllign {
    WidgetAllign_None = 0b00,
    WidgetAllign_Left = 0b01,
    WidgetAllign_Bottom = 0b10,
    WidgetAllign_BottomLeft = 0b11
};

class Widget {
    public:
    Widget(
        WidgetContainer* container,
        int x, int y, int w = 0, int h = 0,
        WidgetAllign allignment = WidgetAllign_None
    );
    virtual ~Widget();

    virtual void Update()=0;
    virtual void Render()=0;

    bool IsOvered();

    //Calculate and update the position of the widget using m_(x/y)offset and m_allignment.
    //Depending of m_allignment, the origin of the offset will be different.
    void UpdatePos();

    //Get the x position of the widget in the container
    int GetX();
    //Get the y position of the widget in the container
    int GetY();
    //Get the position of the widget in the container
    void GetPos(int* x, int* y);


    //Get the x position of the widget in the window
    int GetAbsoluteX();
    //Get the y position of the widget in the window
    int GetAbsoluteY();
    //Get the position of the widget in the window
    void GetAbsolutePosition(int* x, int* y);
    
    protected:
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