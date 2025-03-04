#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class Actor : public GraphObject
{
  public:
    Actor(StudentWorld* sw, int ID, int x, int y);
    virtual void doSomething();
    virtual void getAttacked(Actor * ptr);
    virtual bool canDie() const; // mortals
    virtual bool isEvil() const; // enemies
    virtual bool isClimbable() const; // ladder
    virtual bool isObstruction() const; // floor
    virtual bool isDestructive() const; // bonfires
    virtual bool isGoodie() const; // goodies
    virtual bool isDead() const; // for mortals
    virtual bool canRoll() const {return false;}
    StudentWorld* getWorld() const;

  private:
    StudentWorld* m_world;
};

class Floor : public Actor {
  public:
      Floor(int x, int y);
      // we don't need to explicitly redefine doSomething() in Floor because just like Actor, Floor does nothing
      virtual bool isObstruction() const;
  private: 
      
};

class Attacker : public Actor {
  public:
    Attacker(StudentWorld* sw, int id, int x, int y);
    virtual bool attack();
    virtual bool specialization_attack(Actor * other);
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
    
  private:
  virtual bool special_conditions(Actor * ptr);
    
};

class Mortal : public Attacker {
  public:
    Mortal(StudentWorld * sw, int id, int x, int y);
    virtual bool canDie() const;
    virtual bool isDead() const;
    virtual void setDead(bool dead);
    // virtual bool canMove() const;
  private:
    bool m_alive;
    

};

class Goodie : public Mortal {
  public:
    Goodie(StudentWorld * sw, int id, int x, int y, int points);
    // virtual bool special_conditions(Actor * ptr);
    virtual bool isGoodie() const;
    virtual void doSomething();
    
    
    int getPoints() const;
  private:
    virtual void goodie_special_behavior() = 0; // pure virtual function, must be overriden by specific Goodies
    virtual bool specialization_attack(Actor * ptr);
    int m_points;
    
};

class ExtraLifeGoodie : public Goodie {
  public:
    ExtraLifeGoodie(StudentWorld * sw, int x, int y);
  private:
    virtual void goodie_special_behavior();
};

class GarlicGoodie : public Goodie {
  public:
    GarlicGoodie(StudentWorld * sw, int x, int y);
  private:
    virtual void goodie_special_behavior();

};

// moving mortals have a direction and can move
class MovingMortal : public Mortal {
  public:
    MovingMortal(StudentWorld * sw, int id, int x, int y, int direction);
    virtual bool canMoveThere(int x, int y);
    virtual bool canFallThere(int x, int y);
    virtual bool canClimbThere(int x, int y);
  private:
  
    
};

class Burp : public MovingMortal {
  public:
    Burp(StudentWorld * sw, int x, int y, int dir);
    virtual void doSomething();
    
  private:
    int m_lifetime;
    virtual bool special_conditions(Actor * ptr);

};

class Player : public MovingMortal {
  public:
      Player(StudentWorld* sw, int x, int y);
      virtual void doSomething();
      void setFreezeCount(int count);
      virtual void getAttacked(Actor * ptr);
      void giveBurps(int n);
      int getNumBurps() const;
      
  private: 
      void addBurp();
      int startJump();
      int moveLaterally(int dir);
      int m_freezecount;
      int m_burps;
      int m_jumpcount;
};

class Enemy : public MovingMortal {
  public:
    Enemy(StudentWorld* sw, int id, int x, int y, int dir);
    virtual bool isEvil() const;
    virtual void doSomething();
    virtual void specialized_enemy_actions();
    virtual void getAttacked(Actor * ptr);
    virtual bool timeToDoAction(int n);
  private:
  virtual void special_death_actions() {};
    int m_delay;
    
};

class Kong : public Enemy {
  public:
    Kong(StudentWorld * sw, int x, int y, int dir);
    virtual void getAttacked(Actor * ptr); // do nothing
    // bool isLevelCompleted() const;
  private:
  virtual void specialized_enemy_actions();
    bool m_flee_state;
    int euclidian_dist_w_player();
    void setFlee(bool value);
    int m_time_since_last_barrel;
    int m_barrel_throw_period;
    bool isFleeing() const;
    
};

class Koopa : public Enemy {
  public:
    Koopa(StudentWorld * sw, int x, int y);
  private:
  virtual void special_death_actions();
  virtual void specialized_enemy_actions();
  virtual bool specialization_attack(Actor * ptr);
  virtual bool special_conditions(Actor * ptr);
    int m_freeze_cooldown;
    bool m_just_froze_something;
};

class Fireball : public Enemy {
  public:
    Fireball(StudentWorld * sw, int x, int y);
  private:
    int m_climbing_state; // make constants in GameConstants.h later
    // 0 is not climbing
    // 1 is climbing up
    // -1 is climbing down
    virtual void specialized_enemy_actions();
    virtual void special_death_actions();
    bool isClimbingUp() const;
    bool isClimbingDown() const;
    bool isClimbing() const;
    int getClimbingState() const;
    void setClimbingState(int i);
};

class Barrel : public Enemy {
  public:
    Barrel(StudentWorld * sw, int x, int y, int dir);
    virtual bool canRoll() const {return true;}
    virtual void getAttacked(Actor * ptr);
  private:
    virtual void specialized_enemy_actions();
    
};

#endif // ACTOR_H_
