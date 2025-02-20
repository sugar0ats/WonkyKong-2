#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>
using namespace std;

class Actor;
class Player;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  void setDisplayText(); // set display text to be put on screen
  StudentWorld * getThis();
  Actor * getPtr(int x, int y, Actor* dont_return_me = nullptr, bool only_return_what_can_die = false);
  Player * getPlayer() const;
  // Actor * getPtrExcludingMyself(int x, int y, Actor* dont_return_me);
  void addObject(Actor * ptr);
  int loadLevel(string lvl);
  void setLevelCompleted(bool b);
  bool getLevelCompletedStatus() const;

private:
  Player* m_player;
  vector<Actor*> m_actors;
  bool m_level_completed;
};

#endif // STUDENTWORLD_H_
