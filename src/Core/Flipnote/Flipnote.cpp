#include "Flipnote.h"

#include <SDL.h>
#include <stdio.h>
#include <string>

#include "FlipnoteFrame.h"


//Frames per second for each speed values.
//values taken from https://flipnote.fandom.com/wiki/Flipnote_Speed
static const float s_animationspeedfps[] = {
    0.2,    //0
    0.5,    //1
    1,      //2
    2,      //3
    4,      //4
    6,      //5
    8,      //6
    12,     //7
    20,     //8
    22,     //9
    30      //10
};

static const int s_animationspeedcount = sizeof(s_animationspeedfps) / sizeof(const float);


Flipnote::Flipnote(int framewidth, int frameheight) {
    printf("Flipnote::Flipnote : creating with size %i x %i\n", frameheight, frameheight);
    m_framewidth = framewidth;
    m_frameheight = frameheight;
    SetAnimationSpeed(2);   //1fps by default
    AddFrame(0);
}

Flipnote::~Flipnote() {
    for(FlipnoteFrame* f : m_frames) {
        delete f;
    }
    m_frames.clear();
}

FlipnoteFrame* Flipnote::GetFrame(int index) {
    if(index < 0 || index >= m_frames.size()) return NULL;
    return m_frames[index];
}

void Flipnote::SaveFrame(int index, FlipnoteFrame* newframe) {
    if(index >= FrameCount()) m_frames.push_back(newframe);
    else {
        delete m_frames[index];
        m_frames[index] = newframe;
    }
}


int Flipnote::FrameCount() {
    return m_frames.size();
}


void Flipnote::AddFrame(int index) {
    if(index < 0) index = 0;

    if(index >= FrameCount()) m_frames.push_back(new FlipnoteFrame(m_framewidth, m_frameheight));
    else m_frames.insert(m_frames.begin() + index, new FlipnoteFrame(m_framewidth, m_frameheight));
}

void Flipnote::DeleteFrame(int index) {
    if(index < 0 ||index >= FrameCount()) return;

    m_frames.erase(m_frames.begin() + index);

    if(FrameCount() == 0) AddFrame(0);
}

void Flipnote::MoveFrame(int src, int dest) {
    if(src < 0 || src >= FrameCount()) return;
    if(dest < 0 || dest >= FrameCount()) return;

    FlipnoteFrame* tmp = m_frames[src];

    //Determine in which "direction" we need to move the frame
    int step;
    if(dest < src) step = -1;
    else step = 1;

    //shift all frames by one
    while(src != dest) {
        m_frames[src] = m_frames[src+step];
        src += step;
    }

    //put the frame in the right place
    m_frames[dest] = tmp;
}


void Flipnote::GetFramesSize(int* w, int* h) {
    *w = m_framewidth;
    *h = m_frameheight;
}


///////////////////////////////
// Animation stuff

void Flipnote::SetAnimationSpeed(int speed) {
    if(speed < 0) speed = 0;
    if(speed >= s_animationspeedcount) speed = s_animationspeedcount-1;
    m_animationspeed = speed;

    printf("Flipnote::SetAnimationSpeed : m_animation speed set to %i (correspond to %f fps)\n", m_animationspeed, s_animationspeedfps[m_animationspeed]);
}

int Flipnote::GetAnimationSpeed() {
    return m_animationspeed;
}

float Flipnote::GetAnimationDelay() {
    ////In frame assuming 60fps
    //return 60/s_animationspeedfps[m_animationspeed];
    //Is seconds
    return 1/s_animationspeedfps[m_animationspeed];
}

int Flipnote::GetAnimationFPS() {
    return s_animationspeedfps[m_animationspeed];
}


///////////////////////////////
// Save and load

void Flipnote::Save(const char* filename) {
    if(m_framewidth > 2000 || m_frameheight > 2000) {
        printf("Flipnote::Save : Too big\n");
    }

    //Save signature
    FILE* outfile = fopen(filename, "w");
    fprintf(outfile, "Fnt");

    
    //Save size of a frame
    fputc((m_framewidth >> 8) & 0xff, outfile);
    fputc(m_framewidth & 0xff, outfile);
    fputc((m_frameheight >> 8) & 0xff, outfile);
    fputc(m_frameheight & 0xff, outfile);

    //Save animation speed
    fputc(m_animationspeed, outfile);
    

    //Save number of frames
    unsigned int framecount = FrameCount();
    fputc((framecount >> 8) & 0xff, outfile);
    fputc(framecount & 0xff, outfile);


    //Save all frames
    for(FlipnoteFrame* f : m_frames) {
        f->Save(outfile);
    }

    fclose(outfile);
}

Flipnote* Flipnote::Load(const char* filename) {
    FILE* infile = fopen(filename, "r");
    if(infile == NULL) return nullptr;

    //Read and chack signature
    char signature[4] = {0};
    fgets(signature, 4, infile);
    //printf("Flipnote::Load : %c%c%c\n", signature[0], signature[1], signature[2]);
    if(!(signature[0] == 'F' && signature[1] == 'n' && signature[2] == 't')) {
        printf("Flipnote::Load : No signature\n");
        fclose(infile);
        return nullptr;
    }


    //Get size of frame
    unsigned int w = 0;
    unsigned int h = 0;
    w += fgetc(infile) << 8;
    w += fgetc(infile);
    h += fgetc(infile) << 8;
    h += fgetc(infile);
    printf("Flipnote::Load : w : %i | h : %i\n", w, h);
    if(w > 2000 || h > 2000) {  //Too big
        printf("Flipnote::Load : invalid size (too big)\n");
        fclose(infile);
        return nullptr;
    }

    Flipnote* r = new Flipnote(w, h, infile);
    fclose(infile);
    printf("Flipnote::Load : Successful!\n");
    return r;
}


/////////////////////////
// PRIVATE

//Called by Flipnote::Load
Flipnote::Flipnote(int framewidth, int frameheight, FILE* infile) {
    m_framewidth = framewidth;
    m_frameheight = frameheight;

    //Get animation speed
    SetAnimationSpeed(fgetc(infile));

    //Get frame count
    unsigned int framecount = 0;
    framecount += fgetc(infile) << 8;
    framecount += fgetc(infile);

    //Load all frames
    for(unsigned int i =  0; i < framecount; i++) {
        m_frames.push_back(new FlipnoteFrame(framewidth, frameheight, infile));
    }
}
