#ifndef RUNSTATE_H
#define RUNSTATE_H

#include <map>

struct SDL_Renderer;
struct SDL_Window;

struct RunState {
    SDL_Window* window;
    SDL_Renderer* renderer;

    bool running;

    float mousex;
    float mousey;

    bool rightclick;
    bool leftclick;

    int mousewheel;
    bool mouseused;

    std::map<int, bool> keypressed;
    bool rightpressed;
    bool leftpressed;


    int winwidth;
    int winheight;

    RunState(SDL_Window* win, SDL_Renderer* winrenderer);
    void HandleEvent();

};

#endif