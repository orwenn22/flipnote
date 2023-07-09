#ifndef RUNSTATE_H
#define RUNSTATE_H

#include <map>

struct SDL_Renderer;
struct SDL_RendererInfo;
struct SDL_Window;

//TODO : rename this to a better name. Globals ? EventHandler ? Or something like that ?
struct RunState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_RendererInfo* rendererinfo;

    bool running;

    float mousex;
    float mousey;

    bool rightclick;
    bool leftclick;

    bool rightpressed;
    bool leftpressed;

    int mousewheel;
    bool mouseused;

    std::map<int, bool> keydown;
    std::map<int, bool> keypressed;


    int winwidth;
    int winheight;

    RunState(SDL_Window* win, SDL_Renderer* winrenderer);
    ~RunState();
    void HandleEvent();
    bool IsKeyDown(int key);
    bool IsKeyPressed(int key);
};

//This must be initialised by the program if it needs to be used.
// ex : g_runstate = new RunState()
extern RunState* g_runstate;


#endif