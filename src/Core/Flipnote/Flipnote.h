#ifndef FLIPNOTE_H
#define FLIPNOTE_H

#include <vector>

class FlipnoteFrame;

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Surface;

class Flipnote {
    public:
    //Create a flipnote with a blank frame
    Flipnote(int framewidth, int frameheight);
    ~Flipnote();

    FlipnoteFrame* GetFrame(int index);

    //Overwrite a frame with another (will deallocate existing frame)
    //if index > FrameCount() it will append the frame at the end of m_frames.
    void SaveFrame(int index, FlipnoteFrame* newframe);

    //Get the number of frames in the flipnote
    int FrameCount();

    //if there is already a frame at the index it will shift all the next frames to the right
    //if index > FrameCount() it will append an empty frame at the end of m_frames.
    //if index <0  it will append the frame at the begining of m_frames.
    void AddFrame(int index);

    void DeleteFrame(int index);

    void MoveFrame(int src, int dest);

    void GetFramesSize(int* w, int* h);


    private:
    std::vector<FlipnoteFrame*> m_frames;
    int m_framewidth;
    int m_frameheight;
};

#endif
