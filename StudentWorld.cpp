#include "StudentWorld.h"
#include "Actor.h"
#include "Player.h"
#include "Floor.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{

    // m_player = new Player(this, VIEW_WIDTH / 2, VIEW_HEIGHT / 2);

    loadLevel("level00.txt");
// 1. Initialize the data structures used to keep track of your game’s level and actors.
// 2. Load the current level’s details from the appropriate level data file.
// 3. Allocate and insert a valid Player object into the game world.
// 4. Allocate and insert any Kong objects, Floor objects, Ladder objects, Bonfire objects,
// Fireball objects, Koopa objects, Goodie objects, or other relevant objects into the game
// world, as required by the specification in the current level’s data file.


    // m_player = new Actor(this, VIEW_WIDTH/2, VIEW_HEIGHT/2);

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_player->doSomething();
    for (int i = 0; i < m_actors.size(); i++) {
        m_actors.at(i)->doSomething();
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_player;
    for (int i = 0; i < m_actors.size(); i++) {
        delete m_actors.at(i);
    }
}

void StudentWorld::loadLevel(string lvl) {
    // string curLevel = "level02.txt";
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(lvl);
    if (result == Level::load_fail_file_not_found)
    cerr << "Could not find level00.txt data file\n";
    else if (result == Level::load_fail_bad_format)
    cerr << "Your level was improperly formatted\n";
    else if (result == Level::load_success)
        {
        cerr << "Successfully loaded level\n";
        // otherwise the load was successful and you can access the
        // contents of the level - here's an example
        // int x = 0;
        // int y = 5;
        // Level::MazeEntry item = lev.getContentsOf(x, y);
        // if (item == Level::Player)
        //     cout << "test" << endl;
        for (int x = 0; x < VIEW_HEIGHT; x++) {
            for (int y = 0; y < VIEW_WIDTH; y++) {
                Level::MazeEntry item = lev.getContentsOf(x, y);

                switch (item) {
                    case Level::floor: {
                        cout << "(" << x << "," << y <<  "): " << "is a Floor\n";
                        Floor* m_floor = new Floor(this, x, y);
                        m_actors.push_back(m_floor);
                        break;
                    }
                    // case Level::ladder:
                    //     cout << "(" << x << "," << y << "): " << "is a Ladder\n";
                    //     break;
                    // case Level::left_kong:
                    //     cout << "(" << x << "," << y << "): " << "is a left-facing Kong\n";
                    //     break;
                    // case Level::right_kong:
                    //     cout << "(" << x << "," << y << "): " << "is a right-facing Kong\n";
                    //     break;
                    // case Level::fireball:
                    //     cout << "(" << x << "," << y << "): " << "is a Fireball\n";
                    //     break;
                    // case Level::koopa:
                    //     cout << "(" << x << "," << y << "): " << "is a Koopa\n";
                    //     break;
                    // case Level::bonfire:
                    //     cout << "(" << x << "," << y << "): " << "is a Bonfire\n";
                    //     break;
                    // case Level::extra_life:
                    //     cout << "(" << x << "," << y << "): " << "is a Extra Life Goodie\n";
                    //     break;
                    // case Level::garlic:
                    //     cout << "(" << x << "," << y << "): " << "is a Garlic Goodie\n";
                    //     break;
                    case Level::player: {
                        m_player = new Player(this, x, y);
                        cout << "(" << x << "," << y << "): " << "is a Player starts\n";
                        break;
                    }
                    case Level::empty: {
                        cout << "(" << x << "," << y << "): " << "is a empty\n";
                        break;
                    }
                }
            }
        }
    }
}

Actor* StudentWorld::getPtr(int x, int y) {
    // what if there are multiple things on the same tile?
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->getX() == x && m_actors[i]->getY() == y) {

            return m_actors[i];
        }
    }
    return nullptr;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}