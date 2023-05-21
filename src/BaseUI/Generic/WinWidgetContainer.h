#ifndef WINWIDGETCONTAINER_H
#define WINWIDGETCONTAINER_H

#include "WidgetContainer.h"

//This container is meant to give the illusion that
//the window itself is a widget container.
//(basicly it is the root container)
class WinWidgetContainer : public WidgetContainer {
    public:
    WinWidgetContainer();

    virtual void Update();
    virtual void Render();

    void GetAbsoluteSize(int* w, int* h);
    int GetAbsoluteWidth();
    int GetAbsoluteHeight();

    bool IsMouseOvering();

    void GetRelativeMousePos(int* x, int* y);
};

#endif