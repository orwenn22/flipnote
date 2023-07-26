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

    //Will reload m_filenames
    //Also sets the current page to 0 (aka the first page)
    void ReloadFiles();

    //Get the number of files cached in m_filenames
    int GetFileCount();

    //Get the file name at a specific index of m_filenames
    //return an empty string if index is too small or big
    std::string GetFileName(int index);

    //Get the number of pages based on the file count and the number of element per page
    int GetPageCount();

    //Get the current page index
    int GetCurrentPage();

    //Set the current page index (will check if index is too small or big)
    //Also charged of updating m_pageinfo
    void SetCurrentPage(int index);

    //Returns the number of elements per page
    int ElementPerPage();

    //Get the parrent state of the gallery
    State* ParrentState();

    private:
    std::string m_cwd;                      //Directory in which the gallery look for flipnote files
    State* m_state;                         //Parrent state of the gallery
    WinWidgetContainer* m_widgets;          //In order to have fully managed widgets

    std::vector<std::string> m_filenames;   //Store file names of flipnote files

    //Unloaded by m_widgets
    Label* m_pageinfo;                      //Display the current page and the number of page (ex : "2/5")


    int m_currentpageindex;                 //Current page
    int m_elementperpage;                   //Number of element per page
};

#endif
