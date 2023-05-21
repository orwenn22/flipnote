#ifndef WIDGET_H
#define WIDGET_H

class WidgetContainer;

class Widget {
    public:
    Widget(WidgetContainer* container);
    virtual ~Widget();
    virtual void Update()=0;
    virtual void Render()=0;

    bool IsOvered();

    protected:
    WidgetContainer* m_container;
    int m_x;
    int m_y;
    int m_w;
    int m_h;
};

#endif