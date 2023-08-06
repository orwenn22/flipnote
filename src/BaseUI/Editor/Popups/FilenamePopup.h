#ifndef FLIPNOTE_FILENAMEPOPUP_H
#define FLIPNOTE_FILENAMEPOPUP_H

#include <string>

#include "../../../Reusable/gui/FullscreenPopup.h"

class FlipnoteEditor;

class FilenamePopup : public FullscreenPopup {
public:
    FilenamePopup(FlipnoteEditor* editor);

private:
    FlipnoteEditor* m_editor;
    std::string m_userinput;
};


#endif //FLIPNOTE_FILENAMEPOPUP_H
