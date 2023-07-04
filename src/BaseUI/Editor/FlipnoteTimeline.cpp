#include "FlipnoteTimeline.h"

#include "../../Core/Flipnote/Flipnote.h"
#include "../../Core/Flipnote/FlipnoteFrame.h"
#include "../../Core/FlipnoteRessources.h"
#include "../../Core/Globals.h"
#include "../../Reusable/DeltaTime.h"
#include "../../Reusable/gui/ChildContainer.h"
#include "../../Reusable/gui/IconButton.h"
#include "../../Reusable/RunState.h"
#include "FlipnoteEditor.h"


#include <SDL.h>

#include <stdio.h>



FlipnoteTimeline::FlipnoteTimeline(FlipnoteEditor* editor) {
    m_editor = editor;
    m_y = g_runstate->winheight;

    SDL_QueryTexture(g_ressources->txtr_timelinetile, NULL, NULL, &m_tilewidth, &m_tileheight);

    m_yfrombottomdest = m_tileheight - 38;
    m_yfrombottom = 0;

    m_framesx = FramesXMaxPosition() - 148*m_editor->GetCurrentFrame();
    m_framesxvelocity = 0;

    //Allocate enougth space in the vector for all frames
    m_framestextures = std::vector<SDL_Texture*>(m_editor->GetFlipnote()->FrameCount(), NULL);

    //Setup the container for the timeline buttons (y is calculated by UpdateEnterAnimation)
    m_widgets = new ChildContainer(NULL, 70, 0, g_runstate->winwidth-140, 40, WidgetAllign::WidgetAllign_None);
    m_widgets->m_drawoutline = false;


    ////BUTTONS TO CHANGE THE ORDER OF THE FRAMES

    //Create a new frame
    auto newframebutton = new IconButton(m_widgets, g_ressources->txtr_icon_add, 0, 0);
    newframebutton->SetCallback([&]() { AddFrame(); });
    m_widgets->AddWidget(newframebutton);

    //Delete selected frame
    auto deleteframebutton = new IconButton(m_widgets, g_ressources->txtr_icon_delete, 40, 0);
    deleteframebutton->SetCallback([&]() { DeleteFrame(); });
    m_widgets->AddWidget(deleteframebutton);

    //Move selected frame and put it in the first place
    auto maxleftbutton = new IconButton(m_widgets, g_ressources->txtr_icon_left_double, 80, 0);
    maxleftbutton->SetCallback([&]() { MoveToDest(0); });
    m_widgets->AddWidget(maxleftbutton);

    //Move selected frame to the left
    auto leftbutton = new IconButton(m_widgets, g_ressources->txtr_icon_left, 120, 0);
    leftbutton->SetCallback([&]() { MoveToDest(m_editor->GetCurrentFrame()-1); });
    m_widgets->AddWidget(leftbutton);

    //Move selected frame to the right
    auto rightbutton = new IconButton(m_widgets, g_ressources->txtr_icon_right, 160, 0);
    rightbutton->SetCallback([&]() { MoveToDest(m_editor->GetCurrentFrame()+1); });
    m_widgets->AddWidget(rightbutton);

    //Move selected frame and put it in the last place
    auto maxrightbutton = new IconButton(m_widgets, g_ressources->txtr_icon_right_double, 200, 0);
    maxrightbutton->SetCallback([&]() { MoveToDest(m_editor->GetFlipnote()->FrameCount()-1); });
    m_widgets->AddWidget(maxrightbutton);
}

FlipnoteTimeline::~FlipnoteTimeline() {
    for(SDL_Texture* t : m_framestextures) {
        if(t == NULL) continue;
        SDL_DestroyTexture(t);
    }
    m_framestextures.clear();

    delete m_widgets;
}

void FlipnoteTimeline::Update() {
    //Enter animation (slide from the bottom of the window)
    UpdateEnterAnimation();
    
    //For when we resize the window
    if(m_framesx > FramesXMaxPosition()) m_framesx = FramesXMaxPosition();
    if(m_framesx < FramesXMinPosition()) m_framesx = FramesXMinPosition();


    if(g_runstate->mouseused) return;

    if(IsMouseOvering()) {
        m_widgets->Update();

        //Check if a frame is clicked
        UpdateFrames();
        g_runstate->mouseused = true;
    }
}

void FlipnoteTimeline::Render() {
    //Draw the timeline background (decoration)
    int tileconunt = g_runstate->winwidth / m_tilewidth + 1;
    SDL_FRect dest = {0, (float)m_y, (float)m_tilewidth, (float)m_tileheight};
    for(int i = 0; i < tileconunt; i++) {
        SDL_RenderTexture(g_runstate->renderer, g_ressources->txtr_timelinetile, NULL, &dest);
        dest.x += m_tilewidth;
    }

    RenderFrames();

    m_widgets->Render();
}


bool FlipnoteTimeline::IsMouseOvering() {
    return g_runstate->mousey > m_y + 20;
}


///////////////////////////////
// Button actions

void FlipnoteTimeline::AddFrame() {
    Flipnote* fn = m_editor->GetFlipnote();
    int currentframe = m_editor->GetCurrentFrame();

    //Insert an empty frame right after the current one
    fn->AddFrame(currentframe+1);
    m_framestextures.insert(m_framestextures.begin() + (currentframe+1), NULL);
}

void FlipnoteTimeline::DeleteFrame() {
    Flipnote* fn = m_editor->GetFlipnote();
    int currentframe = m_editor->GetCurrentFrame();

    //Count the number of frame before deleting the current one
    //(If there is only one remaining we need to reload a texture, because it is impossible to heva an empty flipnote)
    int count = fn->FrameCount();

    //Delete current frame
    fn->DeleteFrame(currentframe);
    SDL_DestroyTexture(m_framestextures[currentframe]);
    m_framestextures.erase(m_framestextures.begin() + currentframe);
    if(count == 1) m_framestextures.push_back(NULL);    //Add a texture back if there was only one frame remaining before the deletion

    //Update Editor::m_display
    m_editor->SetCurrentFrame(currentframe);
}

void FlipnoteTimeline::MoveToDest(int dest) {
    int currentframe = m_editor->GetCurrentFrame();
    Flipnote* fn = m_editor->GetFlipnote();

    //Check if the frame is valid
    if(dest < 0 || dest >= fn->FrameCount()) return;

    //Move the frame int the flinote
    fn->MoveFrame(currentframe, dest);

    //Check in which "direction" the frame is going
    int step;
    if(dest < currentframe) step = -1;
    else step = 1;

    //Update the timeline's frames
    //Setting the textures to NULL will make them reload
    for(int i = currentframe; i != dest; i+=step) {
        SDL_DestroyTexture(m_framestextures[i]);
        m_framestextures[i] = NULL;
    }
    SDL_DestroyTexture(m_framestextures[dest]);
    m_framestextures[dest] = NULL;

    //Tell the editor the new position of the frame
    m_editor->SetCurrentFrame(dest);

    //Make it so the frame is at the center of the screen
    m_framesx = FramesXMaxPosition() - 148*m_editor->GetCurrentFrame();
}


///////////////////////////////
// Other stuff

void FlipnoteTimeline::UpdateEnterAnimation() {
    //Make the timeline slide up
    if(m_yfrombottom > m_yfrombottomdest-5) {   //animation over
        m_yfrombottom = (float)m_yfrombottomdest;
    }
    else {      //animation not over
        m_yfrombottom += (600.0*g_deltatime);
    }

    //Calculate absolute position based on m_yfrombottom
    m_y = g_runstate->winheight - (int)m_yfrombottom;

    //Reposition the buttons
    m_widgets->SetYOffest(m_y + 238);
}


int FlipnoteTimeline::FramesXMaxPosition() {
    return (g_runstate->winwidth/2);
}

int FlipnoteTimeline::FramesXMinPosition() {
    return (g_runstate->winwidth/2) - 148*m_editor->GetFlipnote()->FrameCount();
}

void FlipnoteTimeline::UpdateFrames() {
    //Scroll animation
    //FIXME : this is not done correctly
    m_framesxvelocity += (g_runstate->mousewheel * 3180);
    m_framesx += m_framesxvelocity * g_deltatime;
    m_framesxvelocity /= 1.5;
    if(m_framesx > FramesXMaxPosition()) m_framesx = FramesXMaxPosition();
    if(m_framesx < FramesXMinPosition()) m_framesx = FramesXMinPosition();

    //Rect corresponding to the pos of the first frame
    SDL_Rect framerect = {(int)m_framesx, m_y+85, 128, 96};

    //Mouse position
    SDL_Point mousepoint = {
        (int)g_runstate->mousex, 
        (int)g_runstate->mousey
    };

    int i = 0;
    int framecount = m_editor->GetFlipnote()->FrameCount();

    //Ignore the frames that are off screen
    while(framerect.x < -148.0f) {
        framerect.x += 148.0f;  //skip to next frame
        i+=1;
    }

    //Check for click
    while(i < framecount && framerect.x < g_runstate->winwidth) {
        if(SDL_PointInRect(&mousepoint, &framerect) && g_runstate->leftclick) {
            printf("FlipnoteTimeline::UpdateFrames : frame clicked : %i\n", i);
            m_editor->SetCurrentFrame(i);
        }

        framerect.x += 148.0f;  //skip to nerxt frame
        i++;
    }
}


void FlipnoteTimeline::RenderFrames() {
    SDL_SetRenderDrawColor(g_runstate->renderer, 255, 0, 0, 255);

    int i = 0;
    int framecount = m_editor->GetFlipnote()->FrameCount();

    //Rect corresponding to the pos of the first frame
    SDL_FRect framesdest = {(float)m_framesx, (float)(m_y+85), 128.0f, 96.0f};
    
    //Ignore the frames that are off screen
    while(framesdest.x < -148.0f) {
        framesdest.x += 148.0f; //skip to nerxt frame
        i+=1;
    }

    while(i < framecount && framesdest.x < g_runstate->winwidth) {
        LoadAndRenderFrame(i, &framesdest);

        //outline arount selected frame
        if(i == m_editor->GetCurrentFrame()) {
            SDL_FRect recdest = {framesdest.x-4, framesdest.y-4, framesdest.w+8, framesdest.h+8};
            SDL_Color* c = g_ressources->col_orange;
            SDL_SetRenderDrawColor(g_runstate->renderer, c->r, c->g, c->b, c->a);
            SDL_RenderRect(g_runstate->renderer, &recdest);
        }

        framesdest.x += 148.0f; //skip to next frame
        i++;
    }
}

void FlipnoteTimeline::LoadAndRenderFrame(int index, SDL_FRect* dest) {
    if(m_framestextures[index] == NULL) {
        m_framestextures[index] = m_editor->GetFlipnote()->GetFrame(index)->CopyToTexture(128, 96);
    }
    SDL_RenderFillRect(g_runstate->renderer, dest);
    SDL_RenderTexture(g_runstate->renderer, m_framestextures[index], NULL, dest);
}
