#include "StateManager.h"

#include <stdio.h>

#include "State.h"

StateManager::StateManager(State* state) {
    m_state = nullptr;
    m_futurestate = nullptr;
    
    SetState(state);
}

StateManager::~StateManager() {
    if(m_state != nullptr) {
        delete m_state;
        m_state = nullptr;
    }
}


void StateManager::Update() {
    if(m_state != nullptr)
        m_state->Update();
    
    //switch to the new state (if there is one) only once m_state->Update is done
    //FIXME (?) : do this before updating ?
    if(m_futurestate != nullptr) {
        printf("StateManager::Update : switching state\n");
        delete m_state;
        m_state = m_futurestate;
        m_state->SetManager(this);
        m_futurestate = nullptr;
    }
}

void StateManager::Render() {
    if(m_state != nullptr)
        m_state->Render();
}


void StateManager::SetState(State* state) {
    if(state == nullptr) return;

    state->SetManager(this);

    //This system is used to make sure current state finish its Update before switching to the new state
    if(m_state == nullptr) {
        printf("StateManager::SetState : setting state\n");
        m_state = state;
    }
    else {
        printf("StateManager::SetState : switching state when returning to StateManager::Update\n");
        m_futurestate = state;
    }
}
