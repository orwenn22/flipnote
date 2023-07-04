#ifndef DELTATIME_H
#define DELTATIME_H

//In second
extern float g_deltatime;

//Used to calculate DeltaTime
void StartTimingFrame();
void EndTimingFrame();
void EndTimingFrameCappedFramerate(int framerate);

#endif
