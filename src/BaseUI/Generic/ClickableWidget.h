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

    //This is not redefinable. Use PreUpdate() and OnOver() instead.
    void Update() final;
    //Called "before" ClickableWidget::Update().
    virtual void PreUpdate();
    //Called "after" ClickableWidget::Update() if the widget is overred.
    virtual void OnOver();


    virtual void Render();

    void SetCallback(ClickabbleWidgetCallback callback);

    protected:
    //Executed when the widget is clicked
    ClickabbleWidgetCallback m_callback;
};

#endif
