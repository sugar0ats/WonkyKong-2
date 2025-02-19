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
    string str = "level";
    if (getLevel() < 10) {
        str += '0';
        
    } 
    str += (getLevel() + '0');
    str += ".txt";
    loadLevel(str); // loads the file and adds all dynamically allocated objects to the private vector
    cerr << "current level is " << getLevel() << endl;

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
        if (m_actors.at(i) != nullptr) {
            m_actors.at(i)->doSomething(); // force every actor to do something
        }
        
    }

    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors.at(i)->canDie()) {
            if (m_actors.at(i)->isDead()) {
                delete m_actors.at(i);
                cerr << "deleted the guy" << endl;
                m_actors.at(i) = nullptr;
                //m_actors.erase()
                m_actors.erase(m_actors.begin() + i);
            }
        }
    }

    if (m_player != nullptr && m_player->isDead()) {
        if (getLives() == 0) {
            //return GWSTAT
        }
        return GWSTATUS_PLAYER_DIED;
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

    m_actors.clear();
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
                        Floor* m_floor = new Floor(x, y);
                        m_actors.push_back(m_floor);
                        break;
                    }
                    case Level::ladder: {
                        // cout << "(" << x << "," << y << "): " << "is a Ladder\n";
                        Ladder* m_ladder = new Ladder(x, y);
                        m_actors.push_back(m_ladder);
                        break;
                    }
                    case Level::left_kong: {
                        Kong * m_kong = new Kong(this, x, y, GraphObject::right);
                        m_actors.push_back(m_kong);
                        break;
                    }
                    case Level::right_kong: {
                        Kong * m_kong = new Kong(this, x, y, GraphObject::left);
                        m_actors.push_back(m_kong);
                        break;
                    }
                    case Level::fireball: {
                        Fireball * m_fireball = new Fireball(this, x, y);
                        m_actors.push_back(m_fireball);
                        break;
                    }
                    case Level::koopa: {
                        Koopa * m_koopa = new Koopa(this, x, y);
                        m_actors.push_back(m_koopa);
                        break;
                    }
                    case Level::bonfire: {
                        Bonfire * m_bonfire = new Bonfire(this, x, y);
                        m_actors.push_back(m_bonfire);
                        cerr << "bonfire created at " << x << ", " << y << endl;
                        break;
                    }
                        
                    case Level::extra_life: {
                        // xcout << "(" << x << "," << y << "): " << "is a Extra Life Goodie\n";
                        ExtraLifeGoodie * m_extra_life = new ExtraLifeGoodie(this, x, y);
                        m_actors.push_back(m_extra_life);
                        break;
                    }
                    case Level::garlic: {
                        // cout << "(" << x << "," << y << "): " << "is a Garlic Goodie\n";
                        GarlicGoodie * m_garlic = new GarlicGoodie(this, x, y);
                        m_actors.push_back(m_garlic);
                        break;
                    }
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

Actor* StudentWorld::getPtr(int x, int y, Actor* dont_return_me) {
    // what if there are multiple things on the same tile?
    // if (dont_return_me != nullptr) {
    //     cerr << "i am going to skip over me" << endl;
    // }
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors.at(i) != dont_return_me) {
            
            if (m_actors.at(i)->getX() == x && m_actors.at(i)->getY() == y) {
                if (dont_return_me != nullptr) {
                    // cerr << "i am at position (" << dont_return_me->getX() << ", " << dont_return_me->getY() << "): found another at the same position as me" << endl;
                }
                
                // if (m_actors[i] != nullptr && m_actors[i] != dont_return_me) {
                    return m_actors[i];
                // }
                
            }
        }
        
    }

    if (m_player != dont_return_me && m_player->getX() == x && m_player->getY() == y) {
        cerr << "returning the player!" << endl;
        return m_player;
    }

    return nullptr;
}

void StudentWorld::addObject(Actor * ptr) {
    m_actors.push_back(ptr);
}

StudentWorld::~StudentWorld() {
    cleanUp();
}