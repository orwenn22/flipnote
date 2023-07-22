#include "SimpleState.h"

#include "../GraphicalObject.h"

SimpleState::SimpleState() : State() {

}

SimpleState::~SimpleState() {
    for(GraphicalObject* c : m_componants) {
        delete c;
    }
    m_componants.clear();
}

void SimpleState::Update() {
    for(int i = m_componants.size()-1; i >= 0; i--) {
        m_componants[i]->Update();
    }
}

void SimpleState::Render() {
    for(GraphicalObject* c : m_componants) {
        c->Render();
    }
}


void SimpleState::AddComponant(GraphicalObject* componant) {
    if(componant == nullptr) return;
    m_componants.push_back(componant);
}
