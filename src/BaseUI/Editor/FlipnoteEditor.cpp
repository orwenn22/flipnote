#include "FlipnoteEditor.h"

#include <stdio.h>

#include "../../Core/Flipnote/Flipnote.h"
#include "../../Core/Flipnote/FlipnoteFrame.h"
#include "../../Core/Flipnote/FlipnotePainter.h"
#include "../../Core/Ressources.h"
#include "../../Core/RunState.h"
#include "../../Core/Utils.h"
#include "../../Globals.h"
#include "../Generic/PopupMenu.h"
#include "../Generic/WinWidgetContainer.h"
#include "EditorBrushButton.h"
#include "EditorPenButton.h"
#include "EditorTimelineButton.h"
#include "FlipnoteDisplay.h"

#include "FlipnoteTimeline.h"

//#include "../Generic/ChildContainer.h"
//#include "../Generic/CheckBox.h"

static const PaintCondition brushes[] = {
    [](int x, int y) -> bool { return true; },                          // normal
    [](int x, int y) -> bool { return x%2==0; },                        //vertical lines
    [](int x, int y) -> bool { return y%2==0; },                        //horisontal lines

    [](int x, int y) -> bool { return (x%2==0 && y%2==0); },            //dots
    [](int x, int y) -> bool { return (x%2 && y%2); },                  //dots with offset
    [](int x, int y) -> bool { return (x%3==0 && y%3==0); },            //less dots
    [](int x, int y) -> bool { return (x%3==1 && y%3==1); },            //less dots with offset
    [](int x, int y) -> bool { return (x%3 && y%3); },                  //big dots
    [](int x, int y) -> bool { return (x-y)%3 == 0 || (x+y)%3 == 0; },  //wierd grid ?

    [](int x, int y) -> bool { return (x-y)%2 == 0 || (x+y)%2 == 0; },  //paint 1/2 pixels
    [](int x, int y) -> bool { return (x-y)%3 == 0; },                  //diagonal
    [](int x, int y) -> bool { return (x+y)%3 == 0; },                  //other diagonal
    [](int x, int y) -> bool { return (x-y)%4 == 0; },                  //diagonal spaced
    [](int x, int y) -> bool { return (x+y)%4 == 0; },                  //other diagonal spaced
    [](int x, int y) -> bool { return (x-y)%4 == 0 || (x+y)%4 == 0; },  //both diagonals spaces

    [](int x, int y) -> bool { return (x-y)%3 == 0 || (x+y)%2 == 0; },  //weird brush 1
    [](int x, int y) -> bool { return (x+y)%3 == 0 || (x-y)%2 == 0; },  //weird brush 1

    [](int x, int y) -> bool { return SDL_abs(x-y)%3 == 0 || (x+y)%4 == 0; },         //weird brush 2
    [](int x, int y) -> bool { return SDL_abs(x-y)%4 == 0 || (x+y)%3 == 0; },  //weird brush 2


    [](int x, int y) -> bool { return SDL_abs(x-y)%3 == 0 || (x+y)%4 == 0 || SDL_abs(x-y)%4 == 0 || (x+y)%3 == 0; },           //weird brush 2

    //[](int x, int y) -> bool { return x%3==0; },


};
static const int brushcount = sizeof(brushes) / sizeof(PaintCondition);



PaintCondition GetBrush(int index) {
    if(index < 0) index = 0;
    if(index >= brushcount) index = brushcount-1;
    return brushes[index];
}

int GetBrushCount() { return brushcount; }


//bool testbool = true;
//bool testbool2 = true;

FlipnoteEditor::FlipnoteEditor(SDL_Renderer* renderer, Flipnote* fn) {
    m_flipnote = fn;
    //m_flipnote->GetFrame(0)->SetColor(0, {255, 0, 0, 255});
    
    m_page = 0;
    m_currentcolorindex = 1;
    
    m_isdrawing = false;
    m_invertpaint = false;
    m_previousx = -1;
    m_previousy = -1;
    m_currentbrush = 0;
    m_brushsize = 10;

    m_display = new FlipnoteDisplay(renderer, this);

    m_editorbuttons = new WinWidgetContainer();
    m_editorbuttons->AddWidget(new EditorPenButton(m_editorbuttons, this));
    m_editorbuttons->AddWidget(new EditorBrushButton(m_editorbuttons, this));
    m_editorbuttons->AddWidget(new EditorTimelineButton(m_editorbuttons, this));

    ////Test ChildContiner
    //auto cc = new ChildContainer(m_editorbuttons, 30, 30, 200, 200, WidgetAllign_None);
    //cc->AddWidget(new CheckBox(cc, 0, 0, &testbool, "test"));
    //m_editorbuttons->AddWidget(cc);
    //auto cc2 = new ChildContainer(cc, 30, 30, 100, 100, WidgetAllign_None);
    //cc2->m_issolid = true;
    //cc2->AddWidget(new CheckBox(cc2, 0, 0, &testbool2, "test2"));
    //cc->AddWidget(cc2);

    m_popupmenu = NULL;
    m_timeline = NULL;
}

FlipnoteEditor::~FlipnoteEditor() {
    ClosePopupMenu();
    CloseTimeline();
    delete m_editorbuttons;
    delete m_display;
}

void FlipnoteEditor::Update(SDL_Renderer* renderer) {
    //Disable drawing on the canvas when left is released
    if(!g_runstate->leftpressed) {
        m_isdrawing = false;
    }

    ////Update all ui elements
    UpdatePopupMenu();
    m_editorbuttons->Update();
    UpdateTimeline();
    m_display->Update();

    //Draw to the canvas if the user is drawing
    UpdateDraw(renderer);
}

void FlipnoteEditor::Render(SDL_Renderer* renderer) {
    m_display->Render(renderer);
    if(m_timeline != NULL) m_timeline->Render();
    m_editorbuttons->Render();
    if(m_popupmenu != NULL) m_popupmenu->Render(renderer);
}


FlipnoteFrame* FlipnoteEditor::CurrentFrame() {
    return m_flipnote->GetFrame(m_page);
}


void FlipnoteEditor::SetCurrentFrame(int index) {
    int framecount = m_flipnote->FrameCount();
    if(index < 0) index = 0;
    else if(index > framecount) index = framecount-1;

    m_page = index;

    m_display->RefreshTexture(g_runstate->renderer);
}

int FlipnoteEditor::GetCurrentFrame() {
    return m_page;
}


Flipnote* FlipnoteEditor::GetFlipnote() {
    return m_flipnote;
}


int FlipnoteEditor::GetCurrentColorIndex() {
    return m_currentcolorindex;
}

void FlipnoteEditor::SetCurrentColorIndex(int index) {
    if(index < 0) index = 0;
    if(index >= 8) index = 7;       //TODO : 8 color hardcoded here

    m_currentcolorindex = index;
}


int FlipnoteEditor::GetCurrentBrush() {
    return m_currentbrush;
}

void FlipnoteEditor::SetCurrentBrush(int index) {
    if(index < 0) index = 0;
    if(index >= brushcount) index = brushcount-1;
    m_currentbrush = index;
}


int FlipnoteEditor::GetBrushSize() {
    return m_brushsize;
}
void FlipnoteEditor::SetBrushSize(int newsize) {
    if(newsize < 0) newsize = 0;
    m_brushsize = newsize;
}


void FlipnoteEditor::OpenPopupMenu(PopupMenu* popupmenu) {
    if(m_popupmenu != NULL) delete m_popupmenu;
    m_popupmenu = popupmenu;
}

void FlipnoteEditor::ClosePopupMenu() {
    if(m_popupmenu != NULL) {
        delete m_popupmenu;
        m_popupmenu = NULL;
    }
}


void FlipnoteEditor::OpenTimeline() {
    if(m_timeline != NULL) delete m_timeline;
    m_timeline = new FlipnoteTimeline(this);
}

void FlipnoteEditor::CloseTimeline() {
    if(m_timeline != NULL) {
        delete m_timeline;
        m_timeline = NULL;
    }
}

bool FlipnoteEditor::IsTimelineOpen() {
    return (m_timeline != NULL);
}


///////////
//Private

void FlipnoteEditor::UpdateDraw(SDL_Renderer* renderer) {
    if(m_isdrawing) {
        //Get the pixel being overed by the mouse on the canvas
        int pixelx, pixely;
        m_display->GetMousePosPixel(&pixelx, &pixely);

        //We want to draw on the frame and on the display's texture at the same time.
        FlipnotePainter painter(CurrentFrame(), m_display->GetTexture(), GetBrush(m_currentbrush), m_invertpaint);

        if(m_previousx == -1 || m_previousy == -1) {
            //Single click
            painter.FillCircle(pixelx, pixely, m_brushsize, m_currentcolorindex);
        }
        else {
            //Mouse is being held (draw a line between previous and current mouse pos)
            painter.DrawLineEx(m_previousx, m_previousy, pixelx, pixely, m_brushsize, m_currentcolorindex);
        }
        
        printf("%i %i\n", pixelx, pixely);

        //Save the current mouse position for next frame
        m_previousx = pixelx;
        m_previousy = pixely;
    }
    else {
        m_previousx = -1;
        m_previousy = -1;
    }
}


void FlipnoteEditor::UpdatePopupMenu() {
    //Check if there is a popup menu open
    if(m_popupmenu == NULL) return;
 
    m_popupmenu->Update();
        
    //Close popup menu if clicking outside
    if(g_runstate->leftclick && !m_popupmenu->IsMouseOvering() && !g_runstate->mouseused) {
        ClosePopupMenu();
        g_runstate->mouseused = true;
    }
}

void FlipnoteEditor::UpdateTimeline() {
    //Check if the timeline is open
    if(m_timeline == NULL) return;

    m_timeline->Update();

    //Close the timeline if clicking outside
    if(g_runstate->leftclick && !m_timeline->IsMouseOvering() && !g_runstate->mouseused) {
        CloseTimeline();
        g_runstate->mouseused = true;
    }
}
