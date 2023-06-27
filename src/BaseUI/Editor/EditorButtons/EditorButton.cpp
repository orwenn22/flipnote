#include "EditorButton.h"

#include "../../../Core/RunState.h"
#include "../../../Globals.h"

#include <SDL.h>

EditorButton::EditorButton(
    WidgetContainer* container,
    FlipnoteEditor* editor, 
    int x, int y, int w, int h, 
    WidgetAllign allignment, 
    ClickabbleWidgetCallback callback
) 
: ClickableWidget(container, x, y, w, h, allignment, callback) {
    m_editor = editor;
}

EditorButton::~EditorButton() {
    
}
