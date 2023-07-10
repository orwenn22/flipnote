#include "EditorButton.h"

#include "../../../Reusable/RunState.h"

#include <SDL.h>

EditorButton::EditorButton(
    FlipnoteEditor* editor, 
    int x, int y, int w, int h, 
    WidgetAllign allignment, 
    ClickabbleWidgetCallback callback
) 
: ClickableWidget(x, y, w, h, allignment, callback) {
    m_editor = editor;
}

EditorButton::~EditorButton() {
    
}
