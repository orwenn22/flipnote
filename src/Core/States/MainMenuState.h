#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "../../Reusable/State/State.h"

class Background;
class TopBar;
class WinWidgetContainer;

class MainMenuState : public State {
    public:
    MainMenuState();
    ~MainMenuState();

    void Update();
    void Render();

    private:
    Background* m_background;
    TopBar* m_topbar;

    WinWidgetContainer* m_widgets;
};

#endif
