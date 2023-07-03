#include "DeltaTime.h"

#include <SDL.h>
#include <stdio.h>

float g_deltatime = 0;

static Uint32 s_framestart = 0;


void StartTimingFrame() {
    s_framestart = SDL_GetTicks();
}

void EndTimingFrame() {
    //DeltaTime in ms
    Uint32 frameend = SDL_GetTicks();
    Uint32 frametime = frameend - s_framestart;

    if(frametime == 0) return;  //bad timing ?

    //Convert to s
    g_deltatime = (float)frametime / 1000.0f;

}

