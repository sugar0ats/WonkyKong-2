#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class Actor : public GraphObject
{
  public:
    Actor(StudentWorld* sw, int ID, int x, int y);
    virtual void doSomething(); // should i make this virtual?
    virtual void getAttacked();
    virtual bool canDie() const; // mortals
    virtual bool isEvil() const; // enemies
    virtual bool isClimbable() const; // ladder
    virtual bool isObstruction() const; // floor
    virtual bool isDestructive() const; // bonfires
    virtual bool isGoodie() const; // goodie
    virtual bool isDead() const;
    virtual bool isAnimated() const {return false;};
    virtual bool attack() {return false;}
    virtual bool canRoll() {return false;}
    StudentWorld* getWorld() const;

  private:
    StudentWorld* m_world;
};

class Floor : public Actor {
  public:
      Floor(int x, int y);
      //  virtual void doSomething();
      // we don't need to explicitly redefine doSomething() in Floor because just like Actor, Floor does nothing
      virtual bool isObstruction() const;
  private: 
      
};

// class Animated : public Actor {
//   public:
//     Animated(int frames_per_cycle);
//   private:
//     virtual bool isAnimated() const {return true;};
//     void updateAnimation();
// };

class Attacker : public Actor {
  public:
    Attacker(StudentWorld* sw, int id, int x, int y);
    virtual bool attack();
    virtual void specialization_attack(Actor * other);
    virtual bool special_conditions(Actor * ptr);
  private:
};

class Ladder : public Actor {
  public:
    Ladder(int x, int y);
    virtual bool isClimbable() const;
  private:
   
};

class Bonfire : public Attacker {
  public:
    Bonfire(StudentWorld * sw, int x, int y);
    virtual bool isDestructive() const; // override this
    virtual void doSomething();
    virtual bool special_conditions(Actor * ptr);
  private:
    
};

class Mortal : public Attacker {
  public:
    Mortal(StudentWorld * sw, int id, int x, int y);
    virtual bool canDie() const;
    virtual bool isDead() const;
    virtual void setDead(bool dead);
    virtual bool canMove() const;
  private:
    bool m_alive;
    

};

class Goodie : public Mortal {
  public:
    Goodie(StudentWorld * sw, int id, int x, int y);
  private:
    virtual bool isGoodie() const;
};

class ExtraLifeGoodie : public Goodie {
  public:
    ExtraLifeGoodie(StudentWorld * sw, int x, int y);
  private:
};

class GarlicGoodie : public Goodie {
  public:
    GarlicGoodie(StudentWorld * sw, int x, int y);

};

// moving mortals have a direction and can move
class MovingMortal : public Mortal {
  public:
    MovingMortal(StudentWorld * sw, int id, int x, int y, int direction);
    virtual bool canMoveThere(int x, int y);
    virtual bool canFallThere(int x, int y);
    virtual bool canClimbThere(int x, int y);
    virtual bool canMove() const; // override
  private:
    
};

class Burp : public MovingMortal {
  public:
    Burp(StudentWorld * sw, int x, int y, int dir);
    virtual void doSomething();
  private:
    int m_lifetime;

};

class Player : public MovingMortal {
  public:
      Player(StudentWorld* sw, int x, int y);
      virtual void doSomething();
      void setFreezeCount(int count);
      virtual void getAttacked();
      // virtual bool isDead() const;
      // bool canMoveThere(int x, int y);
  private: 
      // bool m_frozen;
      int m_freezecount;
      int m_burps;
      int m_lives; // GameWorld already helps us determine how many lives we have left
      int m_jumpcount;
};

class Enemy : public MovingMortal {
  public:
    Enemy(StudentWorld* sw, int id, int x, int y, int dir);
    virtual bool isEvil() const;
  private:
    
};

class Kong : public Enemy {
  public:
    Kong(StudentWorld * sw, int x, int y, int dir);

};

class Koopa : public Enemy {
  public:
    Koopa(StudentWorld * sw, int x, int y);
};

class Fireball : public Enemy {
  public:
    Fireball(StudentWorld * sw, int x, int y);
};

class Barrel : public Enemy {
  public:
    Barrel(StudentWorld * sw, int x, int y, int dir);
};

#endif // ACTOR_H_
