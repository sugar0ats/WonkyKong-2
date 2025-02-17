#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class Actor : public GraphObject
{
  public:
    Actor(StudentWorld* sw, int ID, int x, int y);
    virtual void doSomething();
    virtual void getAttacked();
    virtual bool canDie() const;
    virtual bool isEvil() const;
    virtual bool isObstruction() const;
    StudentWorld* getWorld();

  private:
    StudentWorld* m_world;
};

#endif // ACTOR_H_
