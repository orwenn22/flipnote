#ifndef WINWIDGETCONTAINER_H
#define WINWIDGETCONTAINER_H

#include "../GraphicalObject.h"
#include "WidgetContainer.h"

//This container is meant to give the illusion that
//the window itself is a widget container.
//(basicly it is the root container, so it can't have a parrent)
class WinWidgetContainer : public WidgetContainer, public GraphicalObject {
    public:
    WinWidgetContainer();

    void Update();
    void Render();

    int GetContainerX();
    int GetContainerY();

    void GetAbsoluteSize(int* w, int* h);
    int GetContainerWidth();
    int GetContainerHeight();

    bool IsMouseOvering();

    void GetRelativeMousePos(int* x, int* y);
};

#endif