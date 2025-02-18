#include "StudentWorld.h"
#include "Actor.h"
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
    // any data members to initialize?
}

int StudentWorld::init()
{
    loadLevel("level00.txt"); // loads the file and adds all dynamically allocated objects to the private vector

// 1. Initialize the data structures used to keep track of your game’s level and actors.
// 2. Load the current level’s details from the appropriate level data file.
// 3. Allocate and insert a valid Player object into the game world.
// 4. Allocate and insert any Kong objects, Floor objects, Ladder objects, Bonfire objects,
// Fireball objects, Koopa objects, Goodie objects, or other relevant objects into the game
// world, as required by the specification in the current level’s data file.

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_player->doSomething(); // force the m_player to do something
    for (int i = 0; i < m_actors.size(); i++) {
        m_actors.at(i)->doSomething(); // force every actor to do something
    }

    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors.at(i)->canDie()) {
            // if (m_actors.at(i)->isDead()) {
            //     delete m_actors.at(i);
            //     m_actors.at(i) = nullptr;
            // }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if (m_player != nullptr) {
        delete m_player;
        m_player = nullptr; // set the pointer to a nullptr so we don't try to delete it again when we call the destructor
    }
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors.at(i) != nullptr) {
            delete m_actors.at(i);
            m_actors.at(i) = nullptr;
        }
        // remember that you can't delete a nullptr!
    }
}

void StudentWorld::loadLevel(string lvl) {
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(lvl);
    if (result == Level::load_fail_file_not_found)
    cerr << "Could not find level00.txt data file\n";
    else if (result == Level::load_fail_bad_format)
    cerr << "Your level was improperly formatted\n";
    else if (result == Level::load_success)
        {
        cerr << "Successfully loaded level\n";
        for (int x = 0; x < VIEW_HEIGHT; x++) {
            for (int y = 0; y < VIEW_WIDTH; y++) {
                Level::MazeEntry item = lev.getContentsOf(x, y);

                switch (item) {
                    case Level::floor: {
                        // cout << "(" << x << "," << y <<  "): " << "is a Floor\n";
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
                        break;
                    }
                    case Level::empty: {
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