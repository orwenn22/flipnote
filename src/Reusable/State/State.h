#ifndef STATE_H
#define STATE_H

class StateManager;

class State {
    public:
    State();
    virtual ~State();

    virtual void Update() = 0;
    virtual void Render() = 0;

    void SetManager(StateManager* manager);
    StateManager* GetManager();

    protected:
    StateManager* m_manager;
};

#endif
