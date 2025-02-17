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
  Actor * getPtr(int x, int y);
  void loadLevel(string lvl);

private:
  Player* m_player;
  vector<Actor*> m_actors;
};

#endif // STUDENTWORLD_H_
