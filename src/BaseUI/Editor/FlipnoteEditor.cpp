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
#include "EditorBrushButton.h"
#include "EditorPenButton.h"
#include "FlipnoteDisplay.h"


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
    m_penbutton = new EditorPenButton(this);
    m_brushbutton = new EditorBrushButton(this);
    m_popupmenu = NULL;
}

FlipnoteEditor::~FlipnoteEditor() {
    ClosePopupMenu();
    delete m_brushbutton;
    delete m_penbutton;
    delete m_display;
}

void FlipnoteEditor::Update(SDL_Renderer* renderer) {
    //Disable drawing on the canvas when left is released
    if(!g_runstate->leftpressed) {
        m_isdrawing = false;
    }

    ////Update all ui elements
    if(m_popupmenu != NULL) {       //Check if there is a popup meun open
        m_popupmenu->Update();
        
        //Close popup menu if clicking outside
        if(g_runstate->leftclick && !m_popupmenu->IsMouseOvering()) {
            ClosePopupMenu();
            g_runstate->mouseused = true;
        }
    }
    m_penbutton->Update();
    m_brushbutton->Update();
    m_display->Update();

    //Draw to the canvas if the user is drawing
    UpdateDraw(renderer);
}

void FlipnoteEditor::Render(SDL_Renderer* renderer) {
    m_display->Render(renderer);
    m_brushbutton->Render();
    m_penbutton->Render();
    if(m_popupmenu != NULL) m_popupmenu->Render(renderer);
}


void FlipnoteEditor::ChangePage(SDL_Renderer* renderer, int index) {
    if(index < 0) index = 0;
    else if(index >= m_flipnote->FrameCount()) index = m_flipnote->FrameCount()-1;
    
    m_page = index;
    m_display->RefreshTexture(renderer);
}


FlipnoteFrame* FlipnoteEditor::CurrentFrame() {
    return m_flipnote->GetFrame(m_page);
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

