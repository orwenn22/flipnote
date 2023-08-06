#include "FlipnoteEditor.h"

#include <stdio.h>

#include "../../Core/Flipnote/Flipnote.h"
#include "../../Core/Flipnote/FlipnoteFrame.h"
#include "../../Core/Flipnote/FlipnotePainter.h"
#include "../../Core/FlipnoteRessources.h"
#include "../../Core/States/MainMenuState.h"
#include "../../Reusable/DeltaTime.h"
#include "../../Reusable/RunState.h"
#include "../../Reusable/gui/FullscreenPopup.h"
#include "../../Reusable/gui/IconButton.h"
#include "../../Reusable/gui/PopupMenu.h"
#include "../../Reusable/State/State.h"
#include "../../Reusable/State/StateManager.h"
#include "../../Reusable/gui/WinWidgetContainer.h"
#include "../../Reusable/Utils.h"
#include "EditorButtons/EditorBrushButton.h"
#include "EditorButtons/EditorPenButton.h"
#include "EditorButtons/EditorPlayButton.h"
#include "EditorButtons/EditorTimelineButton.h"
#include "EditorButtons/LayerButton.h"
#include "EditorButtons/FrogMenuButton.h"
#include "FlipnoteDisplay.h"
#include "FlipnoteTimeline.h"
#include "Popups/FilenamePopup.h"

#include <SDL3/SDL.h>    //FIXME : this is only included for SDL_abs
#include <string>

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

FlipnoteEditor::FlipnoteEditor(Flipnote* fn, std::string filename) {
    m_filename = filename;
    m_flipnote = fn;
    //m_flipnote->SetColor(0, {255, 0, 0, 255});
    
    m_page = 0;
    m_currentcolorindex = 1;
    
    m_isdrawing = false;
    m_needcachedtextureupdate = false;
    m_invertpaint = false;
    m_previousx = -1;
    m_previousy = -1;
    m_currentbrush = 0;
    m_brushsize = 10;

    m_animmationplaying = false;
    m_animationcooldown = 0.0f;

    m_targetlayer = 0;

    m_parentstate = nullptr;

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

    //Layer buttons
    int layer_count = m_flipnote->GetLayerCount();
    for(int i = 0; i < layer_count; i++) {
        m_editorbuttons->AddWidget(new LayerButton(this, i, 10, 65 + 50*i, 50, 50, WidgetAllign_Bottom));

    }

    ////Test ChildContiner
    //auto cc = new ChildContainer(30, 30, 200, 200, WidgetAllign_None);
    //cc->AddWidget(new CheckBox(0, 0, &testbool, "test"));
    //m_editorbuttons->AddWidget(cc);
    //auto cc2 = new ChildContainer(30, 30, 100, 100, WidgetAllign_None);
    //cc2->m_issolid = true;
    //cc2->AddWidget(new CheckBox(0, 0, &testbool2, "test2"));
    //cc->AddWidget(cc2);


    m_popupmenu = nullptr;
    m_timeline = nullptr;

    m_fullscreenpopup = nullptr;
    m_futurfullscreenpopup = nullptr;
    m_fullscreenpopupneeddeletion = false;
    m_fullscreenpopupneedreplacement = false;
}

FlipnoteEditor::~FlipnoteEditor() {
    m_flipnote->Save("autosave.fnt");
    printf("FlipnoteEditor::~FlipnoteEditor : autosaved at autosave.fnt\n");

    ClosePopupMenu();
    CloseTimeline();
    CloseFullscreenPopup();
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

    ////Update all ui elements from top to bottom
    UpdateFullscreenPopup();
    UpdatePopupMenu();
    m_editorbuttons->Update();
    UpdateTimeline();
    m_display->Update();

    //Draw to the canvas if the user is drawing
    UpdateDraw();

    if(g_runstate->IsKeyPressed(SDLK_UP)) m_targetlayer++;
    if(g_runstate->IsKeyPressed(SDLK_DOWN)) m_targetlayer--;

    if(g_runstate->IsKeyPressed(SDLK_ESCAPE)) {
        HandleEscapeKey();
    }

    if(g_runstate->IsKeyDown(SDLK_LCTRL) && g_runstate->IsKeyPressed(SDLK_s)) {
        Save();
    }
}

void FlipnoteEditor::Render() {
    m_display->Render();
    if(m_timeline != nullptr) m_timeline->Render();
    m_editorbuttons->Render();
    if(m_popupmenu != nullptr) m_popupmenu->Render();
    if(m_fullscreenpopup != nullptr) m_fullscreenpopup->Render();
    
    //TODO : replace this bu UI buttons
    //RenderText((std::string("current layer : ") + std::to_string(m_targetlayer)).c_str(), 50, 50, g_ressources->font_ubuntumedium24, {0, 255, 0, 255});

    if(g_runstate->IsKeyDown(SDLK_TAB)) {
        SDL_RenderTexture(g_runstate->renderer, CurrentFrame()->GetCachedTexture(), NULL, NULL);
    }
}


FlipnoteFrame* FlipnoteEditor::CurrentFrame() {
    return m_flipnote->GetFrame(m_page);
}


void FlipnoteEditor::SetCurrentFrame(int index) {
    if(m_isdrawing) return;     //we can't switch frame while drawing (this would cause problems with caching)

    int framecount = m_flipnote->FrameCount();
    if(index < 0) index = 0;
    else if(index >= framecount) index = framecount-1;

    m_page = index;

    auto ms = SDL_GetTicks();
    m_display->RefreshTexture(g_runstate->renderer);
    auto t = SDL_GetTicks() - ms;
    printf("time to switch frame : %li ms\n", t);
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

FlipnoteDisplay* FlipnoteEditor::GetDisplay() {
    return m_display;
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
    if(m_popupmenu != nullptr) delete m_popupmenu;
    m_popupmenu = popupmenu;
}

void FlipnoteEditor::ClosePopupMenu() {
    if(m_popupmenu != nullptr) {
        delete m_popupmenu;
        m_popupmenu = nullptr;
    }
}


void FlipnoteEditor::OpenTimeline() {
    if(m_timeline != nullptr) delete m_timeline;
    m_timeline = new FlipnoteTimeline(this);
}

void FlipnoteEditor::CloseTimeline() {
    if(m_timeline != nullptr) {
        delete m_timeline;
        m_timeline = nullptr;
    }
}

bool FlipnoteEditor::IsTimelineOpen() {
    return (m_timeline != nullptr);
}


void FlipnoteEditor::OpenFullscreenPopup(FullscreenPopup* popup) {
    if(m_fullscreenpopup != nullptr) {  //there is already a fullscreen popup
        m_fullscreenpopupneedreplacement = true;

        //if there is already a future fullscreen popup then replace it by the new one
        if(m_futurfullscreenpopup != nullptr) delete m_futurfullscreenpopup;
        m_futurfullscreenpopup = popup;
    }
    else {
        m_fullscreenpopup = popup;
    }
}

void FlipnoteEditor::CloseFullscreenPopup() {
    if(m_fullscreenpopup != nullptr) {
        m_fullscreenpopupneeddeletion = true;
    }
}

bool FlipnoteEditor::IsFullscreenPopupOpen() {
    return (m_fullscreenpopup != nullptr);
}


void FlipnoteEditor::SetParentState(State* state) {
    if(state == nullptr) return;
    m_parentstate = state;
}

State* FlipnoteEditor::GetParentState() {
    return m_parentstate;
}


void FlipnoteEditor::SetFileName(std::string name) {
    m_filename = name;
}

std::string FlipnoteEditor::GetFileName() {
    return m_filename;
}

void FlipnoteEditor::Save() {
    if(m_filename.empty()) {
        OpenFullscreenPopup(new FilenamePopup(this));
        return;
    }

    m_flipnote->Save(m_filename.c_str());
}

///////////
//Private

void FlipnoteEditor::UpdateDraw() {
    //prevent drawing if animation is playing
    if(m_animmationplaying) return;

    if(m_isdrawing) {
        //Get the pixel being overed by the mouse on the canvas
        int pixelx, pixely;
        m_display->GetMousePosPixel(&pixelx, &pixely);

        //We want to draw on the frame and on the display's texture at the same time.
        FlipnotePainter painter(CurrentFrame(), m_targetlayer, m_display->GetTexture(m_targetlayer), GetBrush(m_currentbrush), m_invertpaint);

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

        //We need to reload the cached texture of current frame when the user finish drawing
        m_needcachedtextureupdate = true;
    }
    else {
        m_previousx = -1;
        m_previousy = -1;

        if(m_needcachedtextureupdate) {
            printf("FlipnoteEditor::UpdateDraw : updated current frame's cached texture\n");
            m_needcachedtextureupdate = false;
            //CurrentFrame()->UpdateCachedTexture();
            //we use the display to update the cashed texture because layers are already loaded as textures
            m_display->OverwriteTexture(CurrentFrame()->GetCachedTexture());
        }
    }
}



void FlipnoteEditor::UpdateAnimation() {
    float animationdelay = m_flipnote->GetAnimationDelay();

    int current_frame = GetCurrentFrame();

    m_animationcooldown+=g_deltatime;
    while(m_animationcooldown >= animationdelay) {
        m_animationcooldown -= animationdelay;
        //Go to next frame
        //(Check if we reached the end of the animation, if so loop to the beginning)
        if(m_page+1 >= m_flipnote->FrameCount()) {
            current_frame = 0;
        }
        else {
            current_frame += 1;
        }
        m_animationcooldown = 0;
    }

    if(current_frame != GetCurrentFrame()) {
        SetCurrentFrame(current_frame);
    }
}


void FlipnoteEditor::UpdatePopupMenu() {
    //Check if there is a popup menu open
    if(m_popupmenu == nullptr) return;
 
    m_popupmenu->Update();
        
    //Close popup menu if clicking outside
    if(g_runstate->leftclick && !m_popupmenu->IsMouseOvering() && !g_runstate->mouseused) {
        ClosePopupMenu();
        g_runstate->mouseused = true;
    }
}

void FlipnoteEditor::UpdateTimeline() {
    //Check if the timeline is open
    if(m_timeline == nullptr) return;

    m_timeline->Update();

    //Close the timeline if clicking outside
    if(g_runstate->leftclick && !m_timeline->IsMouseOvering() && !g_runstate->mouseused) {
        CloseTimeline();
        g_runstate->mouseused = true;
    }
}


void FlipnoteEditor::UpdateFullscreenPopup() {
    if(m_fullscreenpopup != nullptr) m_fullscreenpopup->Update();

    //Most of the time, the fullscreenpopup close itself.
    //That's why we need to fully update it before destroying or replacing it.
    if(m_fullscreenpopupneeddeletion) {
        m_fullscreenpopupneeddeletion = false;
        delete m_fullscreenpopup;
        m_fullscreenpopup = nullptr;
    }

    if(m_fullscreenpopupneedreplacement) {
        m_fullscreenpopupneedreplacement = false;
        delete m_fullscreenpopup;
        m_fullscreenpopup = m_futurfullscreenpopup;
        m_futurfullscreenpopup = nullptr;
    }
}


void FlipnoteEditor::HandleEscapeKey() {
    if(m_fullscreenpopup != nullptr) {
        CloseFullscreenPopup();
        return;
    }

    if(m_popupmenu != nullptr) {    //popupmenu open
        ClosePopupMenu();
        return;
    }

    if(IsTimelineOpen()) {
        CloseTimeline();
        return;
    }

    if(m_parentstate != nullptr) {
        printf("FlipnoteEditor::HandleEscapeKey : Returning to main menu\n");
        m_parentstate->GetManager()->SetState(new MainMenuState());
    }
    else {
        printf("FlipnoteEditor::HandleEscapeKey : The editor don't have a parent state, so we can't spawn the main menu :(\n");
    }
}
