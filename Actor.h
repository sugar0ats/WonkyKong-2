#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class Actor : public GraphObject
{
  public:
    Actor(StudentWorld* sw, int ID, int x, int y);
    virtual void doSomething(); // should i make this virtual?
    virtual void getAttacked(Actor * ptr);
    virtual bool canDie() const; // mortals
    virtual bool isEvil() const; // enemies
    virtual bool isClimbable() const; // ladder
    virtual bool isObstruction() const; // floor
    virtual bool isDestructive() const; // bonfires
    virtual bool isGoodie() const; // goodie
    virtual bool isSmelly() const {return false;}
    virtual bool isExtra() const {return false;}
    virtual bool isDead() const;
    // virtual bool isPlayer() const {};
    virtual bool isAnimated() const {return false;};
    virtual bool attack() {return false;}
    virtual bool canRoll() const {return false;}
    virtual bool canFreezeOthers() const {return false;}
    virtual bool isGas() const {return false;}
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
    Goodie(StudentWorld * sw, int id, int x, int y, int points);
    // virtual bool special_conditions(Actor * ptr);
    virtual bool isGoodie() const;
    virtual void doSomething();
    virtual void specialization_attack(Actor * ptr);
    
    int getPoints() const;
  private:
    int m_points;
    
};

class ExtraLifeGoodie : public Goodie {
  public:
    ExtraLifeGoodie(StudentWorld * sw, int x, int y);
    // virtual void doSomething();
    // virtual void specialization_attack(Actor * other);
    virtual bool isExtra() const {return true;}
  private:
};

class GarlicGoodie : public Goodie {
  public:
    GarlicGoodie(StudentWorld * sw, int x, int y);
    // virtual void doSomething();
    virtual bool isSmelly() const {return true;}
    // virtual bool special_conditions(Actor * ptr);
    // virtual void specialization_attack(Actor * other);

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
    virtual bool special_conditions(Actor * ptr);
    virtual bool isGas() const {return true;}
  private:
    int m_lifetime;

};

class Player : public MovingMortal {
  public:
      Player(StudentWorld* sw, int x, int y);
      virtual void doSomething();
      void setFreezeCount(int count);
      virtual void getAttacked(Actor * ptr);
      void giveBurps(int n);
      void setFrozenTicks(int n);
      int getNumBurps() const;
      
      // virtual bool attack();
      // virtual bool special_conditions(Actor * ptr); 
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
    virtual void getAttacked(Actor * ptr);
    virtual void special_death_actions() {};
    virtual bool timeToDoAction(int n);
  private:
    int m_delay;
    
};

class Kong : public Enemy {
  public:
    Kong(StudentWorld * sw, int x, int y, int dir);
    virtual void doSomething();
    // bool isLevelCompleted() const;
  private:
    bool m_flee_state;
    int euclidian_dist_w_player();
    void setFlee(bool value);
    int m_time_since_last_barrel;
    int m_barrel_throw_period;
    bool isFleeing() const;
    // bool m_level_completed;
    // void setLevelCompleted(bool b);

};

class Koopa : public Enemy {
  public:
    Koopa(StudentWorld * sw, int x, int y);
    virtual void doSomething();
    virtual bool special_conditions(Actor * ptr);
    virtual void specialization_attack(Actor * ptr);
    virtual void special_death_actions();
    virtual bool canFreezeOthers() const {return true;}
  private:
    int m_freeze_cooldown;
    bool m_just_froze_something;
    int m_delay;
};

class Fireball : public Enemy {
  public:
    Fireball(StudentWorld * sw, int x, int y);
    virtual void doSomething();
    virtual void special_death_actions();
    // virtual void getAttacked(Actor * ptr);
  private:
    int m_climbing_state; // make constants in GameConstants.h later
    // 0 is not climbing
    // 1 is climbing up
    // -1 is climbing down
    // int m_delay;
    
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
    virtual void doSomething();
    virtual void getAttacked(Actor * ptr);
    //virtual bool special_conditions(Actor * ptr);
  private:
    int m_delay;
};

#endif // ACTOR_H_
