#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>

#include "BaseUI/DSThumbnail.h"
#include "BaseUI/Editor/FlipnoteEditor.h"
#include "BaseUI/Generic/Background.h"
#include "BaseUI/Generic/TopBar.h"
#include "Core/Ressources.h"
#include "Core/RunState.h"

#include "Core/Utils.h"

#include "Core/Flipnote/Flipnote.h"

RunState* g_runstate;
Ressources* g_ressources;


static SDL_HitTestResult SDLCALL 
MyHitTest(SDL_Window *win, const SDL_Point *area, void *data) {
    int ww;
    int wh;
    SDL_GetWindowSize(win, &ww, &wh);

    if((SDL_GetWindowFlags(g_runstate->window) & SDL_WINDOW_MAXIMIZED) == 0) {      //not maximized
        if(area->y < 7 && area->x < 7)              return SDL_HITTEST_RESIZE_TOPLEFT;
        else if(area->y < 7 && area->x >= ww-7)     return SDL_HITTEST_RESIZE_TOPRIGHT;
        else if(area->y >= wh-7 && area->x < 7)     return SDL_HITTEST_RESIZE_BOTTOMLEFT;
        else if(area->y >= wh-7 && area->x >= ww-7) return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
        else if(area->y < 7)                        return SDL_HITTEST_RESIZE_TOP;
        else if(area->x < 7)                        return SDL_HITTEST_RESIZE_LEFT;
        else if(area->y >= wh-7)                    return SDL_HITTEST_RESIZE_BOTTOM;
        else if(area->x >= ww-7)                    return SDL_HITTEST_RESIZE_RIGHT;
    }
    if(area->y < 30 && area->x < ww-60) {
        return SDL_HITTEST_DRAGGABLE;
    }
    return SDL_HITTEST_NORMAL;
}

int main() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL init failed\n";
        return -1;
    }

    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("Flipnote", 1280, 720, SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE);
    if(!window) {
        std::cout << "win creation failed\n";
        return -1;
    }
    if(SDL_SetWindowMinimumSize(window, 256, 256) < 0) return -1;
    if(SDL_SetWindowHitTest(window, MyHitTest, 0) != 0) return -1;

    SDL_Renderer* renderer = SDL_CreateRenderer(window,NULL, SDL_RENDERER_ACCELERATED);
    if(!renderer) {
        std::cout << "renderer fail\n";
        return -1;
    }

    //Initialise globals
    g_runstate = new RunState(window, renderer);
    g_ressources = new Ressources(renderer);
    
    //Initialise main ui elements
    TopBar* topbar = new TopBar(renderer);
    Background* background = new Background(renderer);

    //Flipnote editor
    Flipnote* flipnote = new Flipnote(512, 384);
    flipnote->AddFrame(1);
    flipnote->AddFrame(1);
    flipnote->AddFrame(1);
    flipnote->AddFrame(1);
    flipnote->AddFrame(1);
    FlipnoteEditor* fe = new FlipnoteEditor(renderer, flipnote);
    

    Uint32 frameStart, frameTime;   //To hava a capped framerate
    while(g_runstate->running) {
        frameStart = SDL_GetTicks();


        g_runstate->HandleEvent();

        //Update
        topbar->Update();
        fe->Update(renderer);
        background->Update(renderer);


        //Render
        background->Render(renderer);
        fe->Render(renderer);
        topbar->Render(renderer);


        SDL_RenderPresent(renderer);

        //Wait before getting to the next frame
        frameTime = SDL_GetTicks() - frameStart;
        if(frameTime < 16) {
            SDL_Delay((int)(16 - frameTime));
        }
    }


    delete fe;
    delete flipnote;

    delete background;
    delete topbar;
    delete g_ressources;
    delete g_runstate;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();

    return 0;
}