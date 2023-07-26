#ifndef FLIPNOTEGALLERY_H
#define FLIPNOTEGALLERY_H

#include "../../Reusable/GraphicalObject.h"

#include <string>
#include <vector>

class Label;
class State;
class WinWidgetContainer;

class FlipnoteGallery : public GraphicalObject {
    public:
    FlipnoteGallery(std::string directory, State* parrentstate);
    ~FlipnoteGallery();

    void Update();
    void Render();

    void ReloadFiles();

    int GetFileCount();
    std::string GetFileName(int index);
    int GetPageCount();
    int GetCurrentPage();
    void SetCurrentPage(int index);

    int ElementPerPage();

    private:
    std::string m_cwd;
    State* m_state;
    WinWidgetContainer* m_widgets;

    std::vector<std::string> m_filenames;

    //Unloaded by m_widgets
    Label* m_pageinfo;


    int m_currentpageindex;
    int m_elementperpage;
};

#endif
