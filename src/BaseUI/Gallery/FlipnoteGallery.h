#ifndef FLIPNOTEGALLERY_H
#define FLIPNOTEGALLERY_H

#include "../../Reusable/GraphicalObject.h"

#include <string>
#include <vector>

class State;
class WinWidgetContainer;

class FlipnoteGallery : public GraphicalObject {
    public:
    FlipnoteGallery(std::string directory, State* parrentstate);
    ~FlipnoteGallery();

    void Update();
    void Render();

    void ReloadFiles();

    int GetPageCount();

    private:
    std::string m_cwd;
    State* m_state;
    WinWidgetContainer* m_widgets;

    std::vector<std::string> m_filenames;

    int m_currentpageindex;
};

#endif
