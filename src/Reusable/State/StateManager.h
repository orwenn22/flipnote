#ifndef STATEMANAGER_H
#define STATEMANAGER_H

class State;

class StateManager {
    public:
    StateManager(State* state = nullptr);
    ~StateManager();

    void Update();
    void Render();

    //If there is already a state, the state change will occure next time Update is called
    void SetState(State* state);

    private:
    State* m_state;
    State* m_futurestate;
};

#endif
