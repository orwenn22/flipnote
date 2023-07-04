#ifndef DELTATIME_H
#define DELTATIME_H

//In second
extern float g_deltatime;

//Used to calculate DeltaTime. Call this at the beginning of the frame.
void StartTimingFrame();

//Update g_deltatime. Call this at the end of the frame.
void EndTimingFrame();

//Wait enought time in order to reach the capped framerate, then update g_deltatime, waiting time included.
//Call this at the end of the frame.
void EndTimingFrameCappedFramerate(int framerate);


//This return the time elipsed since the last time StartTimingFrame was called
float GetCurrentFrameTime();

#endif
