#ifndef POPUPMENUITEM_H
#define POPUPMENUITEM_H

class PopupMenu;

class PopupMenuItem {
    public:
    PopupMenuItem(PopupMenu* popupmenu);
    virtual ~PopupMenuItem();
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