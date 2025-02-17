#ifndef FLOOR_H_
#define FLOOR_H_
#include "Actor.h"

class Floor : public Actor {
    public:
        Floor(StudentWorld* sw, int x, int y);
        virtual void doSomething();
        virtual bool isObstruction() const;
    private: 
        
};

#endif