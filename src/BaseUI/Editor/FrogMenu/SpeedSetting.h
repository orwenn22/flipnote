#ifndef SPEEDSETTING_H
#define SPEEDSETTING_H

#include "../../../Reusable/gui/Widget.h"

class Flipnote;

class SpeedSetting : public Widget {
    public:
    SpeedSetting(Flipnote* flipnote, int x, int y, WidgetAllign allignment = WidgetAllign_None);

    void Update();
    void Render();

    private:
    Flipnote* m_flipnote;
};

#endif
