#ifndef CLICKABLEWIDGET_H
#define CLICKABLEWIDGET_H

#include <functional>

#include "Widget.h"

class ClickableWidget; //forward declaration used by ClickabbleWidgetCallback

using ClickabbleWidgetCallback = std::function<void()>;

class ClickableWidget : public Widget {
    public:
    ClickableWidget(
        WidgetContainer* container,
        int x, int y, int w = 0, int h = 0,
        WidgetAllign allignment = WidgetAllign_None,
        ClickabbleWidgetCallback callback = []() -> void {}
    );
    virtual ~ClickableWidget();

    //Note : a derivated method of ClickableWidget::Update MUST call it.
    virtual void Update();
    virtual void Render();

    void SetCallback(ClickabbleWidgetCallback callback);

    protected:
    //Executed when the button is clicked
    ClickabbleWidgetCallback m_callback;
};

#endif
