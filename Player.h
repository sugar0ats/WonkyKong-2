#ifndef PLAYER_H_
#define PLAYER_H_
#include "Actor.h"

class Player : public Actor {
    public:
        Player(StudentWorld* sw, int x, int y);
        virtual void doSomething();
        bool canMoveThere(int x, int y);
    private: 
        bool frozen;
};

#endif