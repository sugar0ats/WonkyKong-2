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

class Floor : public Actor {
  public:
      Floor(StudentWorld* sw, int x, int y);
      //  virtual void doSomething();
      // we don't need to explicitly redefine doSomething() in Floor because just like Actor, Floor does nothing
      virtual bool isObstruction() const;
  private: 
      
};

class Player : public Actor {
  public:
      Player(StudentWorld* sw, int x, int y);
      virtual void doSomething();
      bool canMoveThere(int x, int y);
  private: 
      bool m_frozen;
      int m_burps;
      int m_lives; // GameWorld already helps us determine how many lives we have left
};

#endif // ACTOR_H_
