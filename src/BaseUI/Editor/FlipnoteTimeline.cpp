#include "FlipnoteTimeline.h"

#include "../../Core/Flipnote/Flipnote.h"
#include "../../Core/Flipnote/FlipnoteFrame.h"
#include "../../Core/Ressources.h"
#include "../../Core/RunState.h"
#include "../../Globals.h"
#include "FlipnoteEditor.h"

#include "../Generic/ChildContainer.h"
#include "../Generic/CheckBox.h"

#include <SDL.h>

#include <stdio.h>

#include "../Generic/ClickableWidget.h"


FlipnoteTimeline::FlipnoteTimeline(FlipnoteEditor* editor) {
    m_editor = editor;
    m_y = g_runstate->winheight;

    SDL_QueryTexture(g_ressources->txtr_timelinetile, NULL, NULL, &m_tilewidth, &m_tileheight);
    m_yfrombottomdest = m_tileheight - 38;
    m_yfrombottom = 0;

    m_framesx = 15;

    //Get all frame textures
    Flipnote* fn = m_editor->GetFlipnote();
    int framecount = fn->FrameCount();
    for(int i = 0; i < framecount; i++) {
        m_framestextures.push_back(fn->GetFrame(i)->CopyToTexture(128, 96));
    }

    //Setup the container for the timeline buttons (y is calculated by UpdateEnterAnimation)
    m_widgets = new ChildContainer(NULL, 70, 0, g_runstate->winwidth-140, 50, WidgetAllign::WidgetAllign_None);
    m_widgets->m_drawoutline = true;
    m_widgets->AddWidget(new ClickableWidget(m_widgets, 0, 0, 20, 20, WidgetAllign_None, [&]() -> void {
        AddFrame();
    }));
}

FlipnoteTimeline::~FlipnoteTimeline() {
    for(SDL_Texture* t : m_framestextures) {
        SDL_DestroyTexture(t);
    }
    m_framestextures.clear();

    delete m_widgets;
}

void FlipnoteTimeline::Update() {
    UpdateEnterAnimation();

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


void FlipnoteTimeline::AddFrame() {
    Flipnote* fn = m_editor->GetFlipnote();
    int currentframe = m_editor->GetCurrentFrame();
    fn->AddFrame(currentframe+1);
    m_framestextures.insert(m_framestextures.begin() + (currentframe+1), fn->GetFrame(currentframe+1)->CopyToTexture(128, 96));
}


void FlipnoteTimeline::UpdateEnterAnimation() {
    //Make the timeline slide up
    if(m_yfrombottom < m_yfrombottomdest) {
        m_yfrombottom += 10;
    } else {
        m_yfrombottom = m_yfrombottomdest;
    }

    m_y = g_runstate->winheight - m_yfrombottom;

    m_widgets->SetYOffest(m_y + 233);
}


void FlipnoteTimeline::UpdateFrames() {
    //Rect corresponding to the pos of the first frame
    SDL_Rect framerect = {m_framesx, m_y+85, 128, 96};

    //Mouse position
    SDL_Point mousepoint = {
        (int)g_runstate->mousex, 
        (int)g_runstate->mousey
    };

    int i = 0;
    int framecount = m_editor->GetFlipnote()->FrameCount();

    //Ignore the frames that are off screen
    while(framerect.x < -148.0f) {
        framerect.x += 148.0f;  //skip to nerxt frame
        i+=1;
    }

    //Check for click
    while(i < framecount && framerect.x < g_runstate->winwidth) {
        if(SDL_PointInRect(&mousepoint, &framerect) && g_runstate->leftclick) {
            printf("frame clicked :%i\n", i);
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
        SDL_RenderFillRect(g_runstate->renderer, &framesdest);
        SDL_RenderTexture(g_runstate->renderer, m_framestextures[i], NULL, &framesdest);

        framesdest.x += 148.0f; //skip to nerxt frame
        i++;
    }
}
