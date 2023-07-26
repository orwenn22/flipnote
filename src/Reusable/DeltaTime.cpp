#include "DeltaTime.h"

#include <SDL3/SDL.h>
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

    //Convert to s
    g_deltatime = (float)frametime / 1000.0f;

}

void EndTimingFrameCappedFramerate(int framerate) {
    //End the deltatime timer here so we can calculate the amount of time to wait in order to cap the framerate
    EndTimingFrame();

    float secperframe = (1.0/(float)framerate);         //time in second between each frame to get the framerate
    if(g_deltatime < secperframe) {                     //check if the current frame's time is less than secperframe
        Uint32 dtms = (Uint32)(g_deltatime*1000.0);     //convert deltatime to ms
        Uint32 spfms = (Uint32)(secperframe*1000.0);    //convert secperfram in ms

        //printf("secperframe : sec : %f   |  ms : %u\n", secperframe, spfms);
        //printf("deltatime   : sec : %f   |  ms : %u\n", g_deltatime, dtms);
        //printf("wait time : %u\n", spfms-dtms);

        SDL_Delay(spfms-dtms);              //if so wait so wait until we reach the correct amount of time (reduce cpu usage)
    }

    //Update deltatime to take into account the waited time
    EndTimingFrame();
}

////old way of waiting for next frame : 
//while(g_deltatime < secperframe) {         //check if the current frame's time is less than secperframe
//    SDL_Delay(2);    //if so wait so wait until we reach the correct amount of time (reduce cpu usage)
//    EndTimingFrame();
//}

float GetCurrentFrameTime() {
    //DeltaTime in ms
    Uint32 frameend = SDL_GetTicks();
    Uint32 frametime = frameend - s_framestart;

    return (float)frametime / 1000.0f;
}
