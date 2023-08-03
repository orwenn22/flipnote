#ifndef SIMPLESTATE_H
#define SIMPLESTATE_H

#include "State.h"

#include <vector>

class GraphicalObject;

class SimpleState : public State {
    public:
    SimpleState();
    virtual ~SimpleState();

    void Update();
    void Render();

    void AddComponant(GraphicalObject* componant);
    void RemoveComponant(GraphicalObject* componant);

    private:
    std::vector<GraphicalObject*> m_componants;
};

#endif
