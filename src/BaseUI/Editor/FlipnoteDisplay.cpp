#include "FlipnoteDisplay.h"

#include <SDL.h>
#include <stdio.h>

#include "../../Core/Flipnote/FlipnoteFrame.h"
#include "../../Core/Flipnote/FlipnotePainter.h"
#include "../../Core/Flipnote/Flipnote.h"
#include "../../Reusable/RunState.h"
#include "FlipnoteEditor.h"

FlipnoteDisplay::FlipnoteDisplay(FlipnoteEditor* editor) {
    m_editor = editor;
    m_x = 50;
    m_y = 50;
    editor->GetFlipnote()->GetFramesSize(&m_unzoomedwidth, &m_unzoomedheight);
    m_scale = 1.0f;

    m_followmouse = false;
    m_mouseoffsetx = 0;
    m_mouseoffsety = 0;

    m_previousframepreview = NULL;
    m_showpreviousframepreview = true;
    RefreshTexture(g_runstate->renderer);

    m_toolpreview = SDL_CreateTexture(g_runstate->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, m_unzoomedwidth, m_unzoomedheight);
    SDL_SetTextureBlendMode(m_toolpreview, SDL_BLENDMODE_BLEND);
}

FlipnoteDisplay::~FlipnoteDisplay() {
    for(SDL_Texture* t : m_currentframetextures) {
        SDL_DestroyTexture(t);
    }
    m_currentframetextures.clear();

    SDL_DestroyTexture(m_toolpreview);
}


void FlipnoteDisplay::Update() {
    //Dragging the canvas
    if(m_followmouse) {
        m_x = g_runstate->mousex - m_mouseoffsetx;
        m_y = g_runstate->mousey - m_mouseoffsety;
    }
    
    ////Don't do anything more if mouse have already been used
    //if(g_runstate->mouseused) return;

    UpdateMouseInput();
}


//TODO : don't draw the offscreen parts of the texture
void FlipnoteDisplay::Render() {
    SDL_FRect dest = {(float)m_x, (float)m_y, (float)m_unzoomedwidth * m_scale, (float)m_unzoomedheight * m_scale};
    SDL_FRect outline;
    if(m_scale > 0.9f) {
        outline = {dest.x - m_scale, dest.y - m_scale, dest.w + m_scale*2, dest.h + m_scale*2};
    }
    else {      //To avoid weird behavior with the outline when unzoomed too much
        outline = {dest.x - 1, dest.y - 1, dest.w + 2, dest.h + 2};
    }


    //draw a gray outline
    SDL_SetRenderDrawColor(g_runstate->renderer, 190, 190, 190, 255);
    SDL_RenderFillRect(g_runstate->renderer, &outline);


    //We want to draw the background color first
    SDL_Color c = m_editor->GetFlipnote()->GetColor(0);
    SDL_SetRenderDrawColor(g_runstate->renderer, c.r, c.g, c.b, 255);   // c.a ?
    SDL_RenderFillRect(g_runstate->renderer, &dest);


    //Draw the preview of the previous frame
    if(m_previousframepreview != NULL && m_showpreviousframepreview) 
        SDL_RenderTexture(g_runstate->renderer, m_previousframepreview, NULL, &dest);


    //Then draw the textures of the layers
    int i_stop = m_currentframetextures.size();
    for(int i = 0; i < i_stop; i++) {
        SDL_RenderTexture(g_runstate->renderer, m_currentframetextures[i], NULL, &dest);

        //Draw the tool preview on top of the selected layer
        if(i == m_editor->m_targetlayer) {
            RenderToolPreview();
            //TODO : maybe draw the grid here
        }
    }

    if(m_scale >= 5) {
        RenderGrid();
    }
}


void FlipnoteDisplay::GetMousePosPixel(int* x, int* y) {
    *x = (int)((g_runstate->mousex - m_x)/m_scale);
    *y = (int)((g_runstate->mousey - m_y)/m_scale);
}

void FlipnoteDisplay::GetMousePosRelative(int* x, int* y) {
    *x = g_runstate->mousex - m_x;
    *y = g_runstate->mousey - m_y;
}


bool FlipnoteDisplay::IsMouseOnDisplay() {
    return (g_runstate->mousex >= m_x && g_runstate->mousey >= m_y
    && g_runstate->mousex < m_x + (int)((float)m_unzoomedwidth*m_scale) 
    && g_runstate->mousey < m_y + (int)((float)m_unzoomedheight*m_scale));
}


bool* FlipnoteDisplay::GetShowPreviousFramePreviewPtr() {
    return &m_showpreviousframepreview;
}


////////////////////////
//Private



void FlipnoteDisplay::UpdateMouseInput() {
    //Disable dragging the canvas when right is released
    if(!g_runstate->rightpressed) {
        m_followmouse = false;
    }

    //Don't do anything more if mouse have already been used
    if(g_runstate->mouseused) return;

    //Enable dragging the canvas when right is pressed
    if(g_runstate->rightclick) {
        m_followmouse = true;
        m_mouseoffsetx = g_runstate->mousex - m_x;
        m_mouseoffsety = g_runstate->mousey - m_y;
    }

    HandleZoom();
    if(IsMouseOnDisplay()) {
        if(g_runstate->leftclick) m_editor->m_isdrawing = true;
    }
}



//Load the surface of the current page into the gpu
void FlipnoteDisplay::RefreshTexture(SDL_Renderer* renderer) {
    printf("FlipnoteDisplay::RefreshTexture : updating textures\n");

    //First time initialisation of the layer textures
    if(m_currentframetextures.size() == 0) {
        //printf("FlipnoteDisplay::RefreshTexture : no existing texture for layers, creating new ones\n");
        m_currentframetextures = m_editor->CurrentFrame()->CopyToTextures();
    }
    else {      //Texture already initialised, so we can just write the new data to them instead of creating new ones
        //printf("FlipnoteDisplay::RefreshTexture : overwritting existing layer textures\n");
        m_editor->CurrentFrame()->OverwriteTextures(m_currentframetextures);
    }


    int currentframeindex = m_editor->GetCurrentFrame();
    if(currentframeindex > 0) {
        if(m_previousframepreview == NULL) {
            //printf("FlipnoteDisplay::RefreshTexture : m_previousframepreview is NULL, creating a new texture\n");
            m_previousframepreview = m_editor->GetFlipnote()->GetFrame(currentframeindex-1)->CopyToTexture(true);
            SDL_SetTextureAlphaMod(m_previousframepreview, 100);
        }
        else {
            //printf("FlipnoteDisplay::RefreshTexture : m_previousframepreview is not NULL, updating the texture\n");
            m_editor->GetFlipnote()->GetFrame(currentframeindex-1)->OverwriteTexture(m_previousframepreview, true);
        }
    }
    else {
        if(m_previousframepreview != NULL) {
            //printf("FlipnoteDisplay::RefreshTexture : current frame is 0, destroying m_previousframepreview\n");
            SDL_DestroyTexture(m_previousframepreview);
            m_previousframepreview = NULL;
        }
    }
}

SDL_Texture* FlipnoteDisplay::GetTexture(int layerindex) {
    int layercount = m_currentframetextures.size();
    if(layercount == 0 || layerindex < 0 || layerindex >= layercount) return nullptr;
    return m_currentframetextures[layerindex];
}


void FlipnoteDisplay::HandleZoom() {
    if(g_runstate->mousewheel != 0) {
        int relativex, relativey;
        GetMousePosRelative(&relativex, &relativey);

        float oldscale = m_scale;

        if(g_runstate->mousewheel > 0) {    //zoom
            m_scale *= 2;
            //Double the distance because we are zooming
            relativex *= 2;
            relativey *= 2;
            //In case someone is dragging and zooming at the same time
            m_mouseoffsetx *= 2;
            m_mouseoffsety *= 2;
        }
        else {  // mousewheel < 0       unzoom
            m_scale /= 2;
            //Divide the distance by 2 because we are unzooming
            relativex /= 2;
            relativey /= 2;
            //In case someone is dragging and zooming at the same time
            m_mouseoffsetx /= 2;
            m_mouseoffsety /= 2;
        }

        if(m_scale < 0.25f || m_scale > 32.0f) {
            m_scale = oldscale;
            return;
        }

        //Actually apply the new distance
        m_x = g_runstate->mousex - relativex;
        m_y = g_runstate->mousey - relativey;
    }
}


void FlipnoteDisplay::RenderGrid() {
    //Find the position of the right and the bottom of the display
    int displayrightpos = m_x + m_unzoomedwidth * m_scale;
    int displaybottompos = m_y + m_unzoomedheight * m_scale;

    //left pixels of the texture that are offscreen
    int offscreenleft = (-m_x) / m_scale; 
    if(offscreenleft < 0) offscreenleft = 0;

    //top pixels of the texture that are offscreen
    int offscreentop = (-m_y) / m_scale;
    if(offscreentop < 0) offscreentop = 0;

    //Find where we need to start drawing the lines
    int firstlinex = m_x + offscreenleft * m_scale;
    int firstliney = m_y + offscreentop * m_scale;

    //Find at what x coordonate we need to stop the lines
    int xstop;
    if(displayrightpos > g_runstate->winwidth) xstop = g_runstate->winwidth;
    else xstop = displayrightpos;

    //Find at what y coordonate we need to stop the lines
    int ystop;
    if(displaybottompos > g_runstate->winheight) ystop = g_runstate->winheight;
    else ystop = displaybottompos;

    //Actually draw the lines
    SDL_SetRenderDrawColor(g_runstate->renderer, 190, 190, 190, 255);   //gray
    for(int x = firstlinex; x < xstop; x += m_scale) {
        SDL_RenderLine(g_runstate->renderer, x, firstliney, x, ystop);
    }
    for(int y = firstliney; y < ystop; y += m_scale) {
        SDL_RenderLine(g_runstate->renderer, firstlinex, y, xstop, y);
    }
}


void FlipnoteDisplay::RenderToolPreview() {
    SDL_Texture* previousrendertarget = SDL_GetRenderTarget(g_runstate->renderer);

    //Get the pixel being overed by the mouse on the canvas
    int pixelx, pixely;
    GetMousePosPixel(&pixelx, &pixely);


    SDL_SetRenderTarget(g_runstate->renderer, m_toolpreview);
    
    SDL_SetRenderDrawColor(g_runstate->renderer, 0, 0, 0, 0);
    SDL_RenderClear(g_runstate->renderer);

    FlipnotePainter painter(m_editor->GetFlipnote()->GetPalette(), m_toolpreview, GetBrush(m_editor->GetCurrentBrush()), m_editor->m_invertpaint);
    painter.FillCircle(pixelx, pixely, m_editor->GetBrushSize(), m_editor->GetCurrentColorIndex());

    SDL_SetRenderTarget(g_runstate->renderer, previousrendertarget);


    SDL_FRect dest = {(float)m_x, (float)m_y, (float)m_unzoomedwidth * m_scale, (float)m_unzoomedheight * m_scale};
    SDL_RenderTexture(g_runstate->renderer, m_toolpreview, NULL, &dest);
}
