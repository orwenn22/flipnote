#include "Flipnote.h"

#include <SDL.h>
#include <stdio.h>

#include "FlipnoteFrame.h"

Flipnote::Flipnote(int framewidth, int frameheight) {
    m_framewidth = framewidth;
    m_frameheight = frameheight;
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
