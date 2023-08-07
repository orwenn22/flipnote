#include "FilenamePopup.h"

#include "../../../Core/FlipnoteRessources.h"
#include "../../../Reusable/gui/Label.h"
#include "../../../Reusable/gui/TextField.h"
#include "../../../Reusable/Utils.h"
#include "../FlipnoteEditor.h"


FilenamePopup::FilenamePopup(FlipnoteEditor *editor) : FullscreenPopup(370, 150) {
    m_editor = editor;
    m_userinput = RemoveExtension(RemovePath(editor->GetFileName()));

    AddWidget(new Label("Enter file name", g_ressources->font_ubuntumedium16, *g_ressources->col_orange, 20, 10));

    AddWidget(new TextField(&m_userinput, 20, 40, 300));
    AddWidget(new Label(".fnt", g_ressources->font_ubuntumedium16, *g_ressources->col_orange, 325, 43));

    AddWidget(
            new Label(
                    "Save", g_ressources->font_ubuntumedium16, *g_ressources->col_orange,
                    30, 120, WidgetAllign_None,
                    [&]() {
                        if(m_userinput.empty()) return;

                        m_userinput += ".fnt";
                        m_editor->SetFileName(m_userinput);
                        m_editor->Save();
                        m_editor->CloseFullscreenPopup();
                    }
            )
    );
}
