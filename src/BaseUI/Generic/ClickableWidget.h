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

    //Note : A derivated method of ClickableWidget::Update MUST call it.
    //       It also need to re-check if the mouse is overring and/or clicking the widget if it provide additionnal interaction
    virtual void Update();
    virtual void Render();

    void SetCallback(ClickabbleWidgetCallback callback);

    protected:
    //Executed when the widget is clicked
    ClickabbleWidgetCallback m_callback;
};

#endif
