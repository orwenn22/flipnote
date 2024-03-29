#include "Flipnote.h"

#include <SDL3/SDL.h>
#include <stdio.h>
#include <string>

#include "../../Reusable/Utils.h"
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


//FIXME : 8 colors hardcoded
static SDL_Color defaultframepalette[] = {
    {255, 255, 255, 255},
    {  0,   0,   0, 255},
    {200,   0,   0, 255},
    {  0,   0, 200, 255},
    {0  , 200,   0, 255},
    {170, 170, 170, 255},
    {200,   0, 200, 255},
    {200, 200,   0, 255},
};

static const int defaultframepalettesize = sizeof(defaultframepalette) / sizeof(defaultframepalette[0]);


Flipnote::Flipnote(int framewidth, int frameheight, int layercount) {
    printf("Flipnote::Flipnote : creating with size %i x %i\n", framewidth, frameheight);
    m_framewidth = framewidth;
    m_frameheight = frameheight;
    m_layercount = layercount;

    //FIXME : 8 colors hardcoded
    m_colors = (SDL_Color*) malloc(sizeof(SDL_Color) * 8);
    m_colors[0] = {255, 255, 255, 255};
    m_colors[1] = {  0,   0,   0, 255};
    m_colors[2] = {200,   0,   0, 255};
    m_colors[3] = {  0,   0, 200, 255};
    m_colors[4] = {  0, 200,   0, 255};
    m_colors[5] = {170, 170, 170, 255};
    m_colors[6] = {200,   0, 200, 255};
    m_colors[7] = {200, 200,   0, 255};

    SetAnimationSpeed(2);   //1fps by default
    AddFrame(0);
}

Flipnote::~Flipnote() {
    for(FlipnoteFrame* f : m_frames) {
        delete f;
    }
    m_frames.clear();

    free(m_colors);
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

    if(index >= FrameCount()) m_frames.push_back(new FlipnoteFrame(this, m_framewidth, m_frameheight));
    else m_frames.insert(m_frames.begin() + index, new FlipnoteFrame(this, m_framewidth, m_frameheight));
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
// Color stuff

void Flipnote::SetColor(int index, SDL_Color c) {
    //FIXME : 8 colors hardcoded
    if(index < 0) index = 0;
    if(index > 7) index = 7;
    m_colors[index] = c;

    //FIXME : this is VERY unoptimised.
    //        it would be better to "dinamicly" update cashed textures when needed
    for(FlipnoteFrame* f : m_frames) {
        f->UpdateCachedTexture();
    }
}

SDL_Color Flipnote::GetColor(int index) {
    //FIXME : 8 colors hardcoded
    if(index < 0) index = 0;
    if(index > 7) index = 7;
    return m_colors[index];
}

SDL_Color* Flipnote::GetPalette() {
    return m_colors;
}

//static
SDL_Color Flipnote::GetDefaultColor(int index) {
    if(index < 0) index = 0;
    if(index > defaultframepalettesize) index = defaultframepalettesize-1;
    return defaultframepalette[index];
}

SDL_Color* Flipnote::GetDefaultPalette() {
    return defaultframepalette;
}


///////////////////////////////
// Other

int Flipnote::GetLayerCount() {
    return m_layercount;
}



///////////////////////////////
// Save and load

void Flipnote::Save(const char* filename) {
    if(m_framewidth > 2000 || m_frameheight > 2000) {
        printf("Flipnote::Save : Too big\n");
    }

    //Save signature
    FILE* outfile = fopen(filename, "w");
    if(outfile == nullptr) {    //If this is true this is most likely because filename is invalid (empty string, etc...)
        printf("Flipnote::Save : failed to create output file :(\n");
        return;
    }
    fprintf(outfile, "Fnt");

    
    //Save size of a frame
    fputc((m_framewidth >> 8) & 0xff, outfile);
    fputc(m_framewidth & 0xff, outfile);
    fputc((m_frameheight >> 8) & 0xff, outfile);
    fputc(m_frameheight & 0xff, outfile);
    printf("Flipnote::Save : w (u16) : %i | h (u16) : %i\n", m_framewidth, m_frameheight);

    //Save number of layers in a frame
    fputc(m_layercount & 0xff, outfile);
    printf("Flipnote::Save : layer count (u8) : %i\n", m_layercount);

    //Save animation speed
    fputc(m_animationspeed & 0xff, outfile);
    printf("Flipnote::Save : animation speed (u8) : %i\n", m_animationspeed);


    //Save number of frames
    unsigned int framecount = FrameCount();
    fputc((framecount >> 8) & 0xff, outfile);
    fputc(framecount & 0xff, outfile);
    printf("Flipnote::Save : number of frames (u16) : %i\n", framecount);


    //Save color palette
    //FIXME : 8 color colors
    printf("Flipnote::Save : saving palette to file : ");
    for(SDL_Color* colptr = m_colors; colptr < m_colors + 8;colptr += 1) {
        putc(colptr->r, outfile);
        putc(colptr->g, outfile);
        putc(colptr->b, outfile);
        putc(colptr->a, outfile);
        printf("\x1b[48;2;%u;%u;%um  \x1b[0m", colptr->r, colptr->g, colptr->b);    //ANSI escape code for changing terminal's background color
    }
    printf("\n");


    //Save all frames
    for(FlipnoteFrame* f : m_frames) {
        f->Save(outfile);
    }

    fclose(outfile);
    printf("Flipnote::Save : Successfull !\n");
}

Flipnote* Flipnote::Load(const char* filename, bool onlyfirstframe) {
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

    Flipnote* r = new Flipnote(w, h, infile, onlyfirstframe);
    fclose(infile);
    printf("Flipnote::Load : Successful!\n");
    return r;
}


/////////////////////////
// PRIVATE

//Called by Flipnote::Load
Flipnote::Flipnote(int framewidth, int frameheight, FILE* infile, bool onlyfirstframe) {
    m_framewidth = framewidth;
    m_frameheight = frameheight;

    //Get layer count
    m_layercount = fgetc(infile);
    printf("Flipnote::Flipnote : layer count from file : %i\n", m_layercount);

    //Get animation speed
    SetAnimationSpeed(fgetc(infile));
    printf("Flipnote::Flipnote : animation speed from file : %i\n", m_animationspeed);

    //Get number of frames
    unsigned int framecount = 0;
    framecount += fgetc(infile) << 8;
    framecount += fgetc(infile);
    printf("Flipnote::Flipnote : number of frames from file : %i\n", framecount);

    //Get colors
    //FIXME : 8 colors hardcoded
    m_colors = (SDL_Color*) malloc(sizeof(SDL_Color) * 8);
    printf("Flipnote::Flipnote : loading palette from file : ");
    for(SDL_Color* colptr = m_colors; colptr < m_colors + 8;colptr += 1) {
        colptr->r = getc(infile);
        colptr->g = getc(infile);
        colptr->b = getc(infile);
        colptr->a = getc(infile);
        printf("\x1b[48;2;%u;%u;%um  \x1b[0m", colptr->r, colptr->g, colptr->b);    //ANSI escape code for changing terminal's background color
    }
    printf("\n");

    //Load one or all frames
    if(onlyfirstframe) framecount = 1;
    for(unsigned int i =  0; i < framecount; i++) {
        m_frames.push_back(new FlipnoteFrame(this, framewidth, frameheight, infile));
    }
}


bool CheckFlipnoteFile(std::string path) {
	if(path.size() <= 4) return false;
	if(path.substr(path.size()-4) != ".fnt") return false;
	
	FILE* infile = fopen(path.c_str(), "r");
	if(infile == NULL) return false;

	char buf[4] = { 0 };
	freadbytes(buf, 4, infile);
	fclose(infile);

	if(std::string(buf) != "Fnt") return false;

	return true;	
}
