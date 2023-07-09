#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>

#include "BaseUI/DSThumbnail.h"
#include "BaseUI/Editor/FlipnoteEditor.h"
#include "Core/FlipnoteRessources.h"
#include "Core/States/EditorState.h"
#include "Core/States/MainMenuState.h"
#include "Reusable/DebugOverlay.h"
#include "Reusable/DeltaTime.h"
#include "Reusable/gui/TopBar.h"
#include "Reusable/RunState.h"
#include "Reusable/State/StateManager.h"
#include "Reusable/Utils.h"

#include "Core/Flipnote/Flipnote.h"


int main(int argc, const char* argv[]) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL init failed\n";
        return -1;
    }

    for(int i = 0; i < SDL_GetNumRenderDrivers(); i++) {
        printf("main : %i : %s\n", i, SDL_GetRenderDriver(i));
    }

    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("Flipnote", 1280, 720, SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE);
    if(!window) {
        std::cout << "win creation failed\n";
        return -1;
    }
    if(SDL_SetWindowMinimumSize(window, 256, 256) < 0) return -1;
    if(SDL_SetWindowHitTest(window, BorderlessHitTest, 0) != 0) return -1;

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED);
    if(!renderer) {
        std::cout << "renderer fail\n";
        return -1;
    }


    //Initialise globals
    g_runstate = new RunState(window, renderer);
    g_ressources = new FlipnoteRessources(renderer);
    g_reusableressources = g_ressources;
    

    //StateManger
    StateManager* state_manager = new StateManager();

    //Check if we are loading a flipnote from argv1
    Flipnote* flipnote = nullptr;
    if(argc == 2) {
        printf("main : trying to load flipnote from '%s'\n", argv[1]);
        flipnote = Flipnote::Load(argv[1]);
        if(flipnote == nullptr) {
            printf("main : failed to load flipnote from '%s'\n", argv[1]);
        }
    }

    
    if(flipnote != nullptr) state_manager->SetState(new EditorState(flipnote));
    else state_manager->SetState(new MainMenuState());
    


    bool showdebugoverlay = false;
    

    while(g_runstate->running) {
        StartTimingFrame();


        g_runstate->HandleEvent();
        if(g_runstate->IsKeyPressed(SDLK_F3)) showdebugoverlay = !showdebugoverlay;

        //Update
        state_manager->Update();


        //Render
        state_manager->Render();


        if(showdebugoverlay) RenderDebugOverlay(0, 0);


        SDL_RenderPresent(renderer);

        //EndTimingFrame();       //for uncapped framerate
        EndTimingFrameCappedFramerate(120);

        
        //const float secperframe = (1.0/120.0);  //time in second between each frame to get 120 fps
        //printf("main : time waited : %fs  |  target : %fs\n", g_deltatime, secperframe);
    }


    delete state_manager;

    delete g_ressources;
    delete g_runstate;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();

    return 0;
}
