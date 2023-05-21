#ifndef WIDGET_H
#define WIDGET_H

class PopupMenu;

class Widget {
    public:
    Widget(PopupMenu* popupmenu);
    virtual ~Widget();
    virtual void Update()=0;
    virtual void Render()=0;

    bool IsOvered();

    protected:
    PopupMenu* m_popupmenu;
    int m_x;
    int m_y;
    int m_w;
    int m_h;
};

#endif