#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>

#include "BaseUI/DSThumbnail.h"
#include "BaseUI/Editor/FlipnoteEditor.h"
#include "BaseUI/Generic/Background.h"
#include "Core/FlipnoteRessources.h"
#include "Reusable/DeltaTime.h"
#include "Reusable/gui/TopBar.h"
#include "Reusable/RunState.h"

#include "Core/Utils.h"

#include "Core/Flipnote/Flipnote.h"


//Globals definition
RunState* g_runstate;
Ressources* g_reusableressources;
FlipnoteRessources* g_ressources;


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

int main(int argc, const char* argv[]) {
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
    g_ressources = new FlipnoteRessources(renderer);
    g_reusableressources = g_ressources;
    
    //Initialise main ui elements
    TopBar* topbar = new TopBar(renderer);
    Background* background = new Background(renderer);

    //Flipnote (canvas)
    Flipnote* flipnote;
    if(argc == 1) {
        flipnote = new Flipnote(512, 384);
    }
    else  {
        flipnote = Flipnote::Load(argv[1]);
        printf("main : trying to load flipnote from '%s'\n", argv[1]);
        if(flipnote == nullptr) {
            printf("main : failed to load flipnote from '%s'\n", argv[1]);
            flipnote = new Flipnote(512, 384);
        }
    }

    //Flipnote Editor
    FlipnoteEditor* fe = new FlipnoteEditor(renderer, flipnote);
    

    while(g_runstate->running) {
        StartTimingFrame();

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

        const float secperframe = (1.0/120.0);  //time in second between each frame to get 120 fps
        if(g_deltatime < secperframe) {         //check if the current frame's time is less than secperframe
            Uint32 dtms = (Uint32)(g_deltatime*1000.0);
            Uint32 spfms = (Uint32)(secperframe*1000.0);
            SDL_Delay(spfms-dtms);              //if so wait so wait until we reach the correct amount of time (reduce cpu usage)
        }
        

        EndTimingFrame();
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