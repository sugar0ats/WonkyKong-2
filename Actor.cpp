#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

Actor::Actor(StudentWorld* sw, int ID, int x, int y)
 : GraphObject(ID, x, y), m_world(sw) // direction is a default parameter that is set to none
{
}

void Actor::doSomething()
{
    // int key;
    // if (m_world->getKey(key))
    // {
    //     if (key == KEY_PRESS_SPACE)
    //         m_world->playSound(SOUND_GOT_GOODIE);
    // }
}

void Actor::getAttacked() {
    // stub
}

bool Actor::canDie() const {
    return false;
}

bool Actor::isEvil() const {
    return false;
}

bool Actor::isObstruction() const {
    return false;
}

StudentWorld* Actor::getWorld() {
    return m_world;
}