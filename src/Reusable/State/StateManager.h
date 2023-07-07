#ifndef STATEMANAGER_H
#define STATEMANAGER_H

class State;

class StateManager {
    public:
    StateManager(State* state = nullptr);
    ~StateManager();

    void Update();
    void Render();

    void SetState(State* state);

    private:
    State* m_state;
    State* m_futurestate;
};

#endif
