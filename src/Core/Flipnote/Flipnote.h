#ifndef FLIPNOTE_H
#define FLIPNOTE_H

#include <bits/types/FILE.h>
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


    ///////////////////////////////////////////////
    // Frames operation

    FlipnoteFrame* GetFrame(int index);

    //Overwrite a frame with another (will deallocate existing frame)
    //if index > FrameCount() it will append the frame at the end of m_frames.
    void SaveFrame(int index, FlipnoteFrame* newframe);     //TODO : remove this because it is useless (for now?)

    //Get the number of frames in the flipnote
    int FrameCount();

    //if there is already a frame at the index it will shift all the next frames to the right
    //if index > FrameCount() it will append an empty frame at the end of m_frames.
    //if index <0  it will append the frame at the begining of m_frames.
    void AddFrame(int index);

    void DeleteFrame(int index);

    void MoveFrame(int src, int dest);

    void GetFramesSize(int* w, int* h);


    ///////////////////////////////////////////////
    //Animation speed related things

    //Sets m_animationspeed
    void SetAnimationSpeed(int speed);
    //Returns m_animationspeed
    int GetAnimationSpeed();
    //Return the time (in seconds) between each frame
    float GetAnimationDelay();
    //Return the animation's frames per seconds
    int GetAnimationFPS();


    ///////////////////////////////////////////////
    //Saving and loading

    void Save(const char* filename);
    static Flipnote* Load(const char* filename);


    private:
    //called by Flipnote::Load
    Flipnote(int framewidth, int frameheight, FILE* infile);


    std::vector<FlipnoteFrame*> m_frames;
    unsigned int m_framewidth;
    unsigned int m_frameheight;
    unsigned char m_animationspeed;  //this is the index corresponding to a value in s_animationspeedfps
};

#endif
