#ifndef DEBUGOVERLAY_H
#define DEBUGOVERLAY_H

//A basic overlay that shows :
// -DeltaTime (the time it took to render the previous frame, including the waiting time of the caped framerate)
// -FrameTime (the time it took to render this frame without waiting)
void RenderDebugOverlay(int basex, int basey);

#endif
