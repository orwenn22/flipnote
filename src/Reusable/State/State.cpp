#include "State.h"

State::State() {
    m_manager = nullptr;
}

State::~State() {}

void State::SetManager(StateManager* manager) {
    m_manager = manager;
}

StateManager* State::GetManager() {
    return m_manager;
}
