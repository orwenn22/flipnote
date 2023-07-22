#ifndef GRAPHICALOBJECT_H
#define GRAPHICALOBJECT_H

class GraphicalObject {
    public:
    GraphicalObject(){};
    virtual ~GraphicalObject(){};

    virtual void Update() = 0;
    virtual void Render() = 0;
};

#endif
