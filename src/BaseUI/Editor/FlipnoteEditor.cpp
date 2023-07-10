#include "FlipnoteEditor.h"

#include <stdio.h>

#include "../../Core/Flipnote/Flipnote.h"
#include "../../Core/Flipnote/FlipnoteFrame.h"
#include "../../Core/Flipnote/FlipnotePainter.h"
#include "../../Core/FlipnoteRessources.h"
#include "../../Reusable/DeltaTime.h"
#include "../../Reusable/RunState.h"
#include "../../Reusable/gui/IconButton.h"
#include "../../Reusable/gui/PopupMenu.h"
#include "../../Reusable/gui/WinWidgetContainer.h"
#include "EditorButtons/EditorBrushButton.h"
#include "EditorButtons/EditorPenButton.h"
#include "EditorButtons/EditorPlayButton.h"
#include "EditorButtons/EditorTimelineButton.h"
#include "EditorButtons/FrogMenuButton.h"
#include "FlipnoteDisplay.h"
#include "FlipnoteTimeline.h"

#include <SDL.h>    //FIXME : this is only included for SDL_abs

//#include "../../Reusable/gui/ChildContainer.h"
//#include "../../Reusable/gui/CheckBox.h"

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

FlipnoteEditor::FlipnoteEditor(Flipnote* fn) {
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

    m_animmationplaying = false;
    m_animationcooldown = 0.0f;

    m_display = new FlipnoteDisplay(this);


    ////Widgets
    m_editorbuttons = new WinWidgetContainer();

    //Tools
    m_editorbuttons->AddWidget(new EditorPenButton(this));
    m_editorbuttons->AddWidget(new EditorBrushButton(this, 10, 95, WidgetAllign_Left));

    //Play controls
    m_editorbuttons->AddWidget(new IconButton(g_ressources->txtr_button_playback_begginning, 250, 15, 0, 0, WidgetAllign_BottomLeft, [&]() { SetCurrentFrame(0); }));
    m_editorbuttons->AddWidget(new IconButton(g_ressources->txtr_button_playback, 210, 15, 0, 0, WidgetAllign_BottomLeft, [&]() { GoToPreviousFrame(); }));
    m_editorbuttons->AddWidget(new EditorPlayButton(this, 155, 10, WidgetAllign_BottomLeft));
    m_editorbuttons->AddWidget(new IconButton(g_ressources->txtr_button_playforward, 110, 15, 0, 0, WidgetAllign_BottomLeft, [&]() { GoToNextFrame(); }));
    m_editorbuttons->AddWidget(new IconButton(g_ressources->txtr_button_playforward_end, 70, 15, 0, 0, WidgetAllign_BottomLeft, [&]() { SetCurrentFrame(GetFlipnote()->FrameCount() - 1); }));
    
    m_editorbuttons->AddWidget(new EditorTimelineButton(this, 10, 10, WidgetAllign_BottomLeft));

    //Frog Button
    m_editorbuttons->AddWidget(new FrogMenuButton(this, 10, 10, WidgetAllign_Bottom));

    ////Test ChildContiner
    //auto cc = new ChildContainer(30, 30, 200, 200, WidgetAllign_None);
    //cc->AddWidget(new CheckBox(0, 0, &testbool, "test"));
    //m_editorbuttons->AddWidget(cc);
    //auto cc2 = new ChildContainer(30, 30, 100, 100, WidgetAllign_None);
    //cc2->m_issolid = true;
    //cc2->AddWidget(new CheckBox(0, 0, &testbool2, "test2"));
    //cc->AddWidget(cc2);


    m_popupmenu = NULL;
    m_timeline = NULL;
}

FlipnoteEditor::~FlipnoteEditor() {
    m_flipnote->Save("autosave.fnt");
    printf("FlipnoteEditor::~FlipnoteEditor : autosaved at autosave.fnt\n");

    ClosePopupMenu();
    CloseTimeline();
    delete m_editorbuttons;
    delete m_display;
}

void FlipnoteEditor::Update() {
    //Disable drawing on the canvas when left is released
    if(!g_runstate->leftpressed) {
        m_isdrawing = false;
    }

    //Update animation
    if(m_animmationplaying) {
        UpdateAnimation();
    }

    ////Update all ui elements
    UpdatePopupMenu();
    m_editorbuttons->Update();
    UpdateTimeline();
    m_display->Update();

    //Draw to the canvas if the user is drawing
    UpdateDraw();
}

void FlipnoteEditor::Render() {
    m_display->Render();
    if(m_timeline != NULL) m_timeline->Render();
    m_editorbuttons->Render();
    if(m_popupmenu != NULL) m_popupmenu->Render();
}


FlipnoteFrame* FlipnoteEditor::CurrentFrame() {
    return m_flipnote->GetFrame(m_page);
}


void FlipnoteEditor::SetCurrentFrame(int index) {
    int framecount = m_flipnote->FrameCount();
    if(index < 0) index = 0;
    else if(index >= framecount) index = framecount-1;

    m_page = index;

    m_display->RefreshTexture(g_runstate->renderer);
}

int FlipnoteEditor::GetCurrentFrame() {
    return m_page;
}

void FlipnoteEditor::GoToNextFrame() {
    if(m_page+1 < m_flipnote->FrameCount()) {
        SetCurrentFrame(m_page+1);
    }
}
void FlipnoteEditor::GoToPreviousFrame() {
    if(m_page > 0) {
        SetCurrentFrame(m_page-1);
    }
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

void FlipnoteEditor::UpdateDraw() {
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
        
        printf("FlipnoteEditor::UpdateDraw : %i %i\n", pixelx, pixely);

        //Save the current mouse position for next frame
        m_previousx = pixelx;
        m_previousy = pixely;
    }
    else {
        m_previousx = -1;
        m_previousy = -1;
    }
}


/*
    m_animationcooldown++;
    if(m_animationcooldown >= m_animationinterval) {
        m_animationcooldown = 0;

        //Check if we reached the end of the animation
        if(m_page+1 >= m_flipnote->FrameCount()) {
            SetCurrentFrame(0);
        }
        else {
            SetCurrentFrame(m_page+1);
        }
    }
*/

void FlipnoteEditor::UpdateAnimation() {
    float animationdelay = m_flipnote->GetAnimationDelay();

    m_animationcooldown+=g_deltatime;
    while(m_animationcooldown >= animationdelay) {
        m_animationcooldown -= animationdelay;
        //Go to next frame
        //(Check if we reached the end of the animation, if so loop to the beginning)
        if(m_page+1 >= m_flipnote->FrameCount()) {
            SetCurrentFrame(0);
        }
        else {
            SetCurrentFrame(m_page+1);
        }
        m_animationcooldown = 0;
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
