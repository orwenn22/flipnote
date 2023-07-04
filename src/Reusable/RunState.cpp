#include "RunState.h"

#include <SDL.h>
#include <SDL_keycode.h>


RunState::RunState(SDL_Window* win, SDL_Renderer* winrenderer) {
    window = win;
    renderer = winrenderer;
    running = true;
    mousex = 0;
    mousey = 0;
    mousewheel = 0;
    rightpressed = false;
    leftpressed = false;
    rightclick = false;
    leftclick = false;
    mouseused = false;
    SDL_GetWindowSize(window, &winwidth, &winheight);

    rendererinfo = new SDL_RendererInfo;
    SDL_GetRendererInfo(renderer, rendererinfo);
}

RunState::~RunState() {
    delete rendererinfo;
}


void RunState::HandleEvent() {
    keypressed.clear();
    rightclick = false;
    leftclick = false;
    mouseused = false;
    mousewheel = 0;

    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_EVENT_QUIT:
                running = false;
            break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    if(event.button.button == SDL_BUTTON_LEFT) {
                        leftclick = !leftpressed;
                        leftpressed = true;
                    }
                    if(event.button.button == SDL_BUTTON_RIGHT) {
                        rightclick = !rightpressed;
                        rightpressed = true;
                    }
                }
            break;
            
            case SDL_EVENT_MOUSE_BUTTON_UP:
                {
                    if(event.button.button == SDL_BUTTON_LEFT) {
                        leftpressed = false;
                    }
                    if(event.button.button == SDL_BUTTON_RIGHT) {
                        rightpressed = false;
                    }
                }
            break;

            case SDL_EVENT_KEY_DOWN:
                keydown[event.key.keysym.sym] = true;
                keypressed[event.key.keysym.sym] = true;
            break;

            case SDL_EVENT_KEY_UP:
                keydown[event.key.keysym.sym] = false;
            break;

            case SDL_EVENT_MOUSE_WHEEL:
                mousewheel += event.wheel.y;
            break;
        }
    }

    SDL_GetMouseState(&mousex, &mousey);
    SDL_GetWindowSize(window, &winwidth, &winheight);
}


bool RunState::IsKeyDown(int key) {
    if(keydown.count(key) == 0) return false;

    return keydown[key];
}

bool RunState::IsKeyPressed(int key) {
    if(keydown.count(key) == 0) return false;

    return keypressed[key];
}
