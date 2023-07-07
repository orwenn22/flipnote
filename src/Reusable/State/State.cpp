#include "State.h"

State::State() {}

State::~State() {}

void State::SetManager(StateManager* manager) {
    m_manager = manager;
}
