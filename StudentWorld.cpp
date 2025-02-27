#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_level_completed(false)
{
    // any data members to initialize?
}

int StudentWorld::init()
{
    // DEBUG
    if (getLevel() == 100) { // TEMPORARILY, ORIGINALLY 100 
        return GWSTATUS_PLAYER_WON;
    }
    string str = "level";
    if (getLevel() < 10) {
        str += '0';
        
    } 
    str += (getLevel() + '0');
    str += ".txt";
    int result = loadLevel(str); // loads the file and adds all dynamically allocated objects to the private vector
    // int result = loadLevel("level02.txt");
    cerr << "current level is " << getLevel() << endl;

    // debug region:
    // m_actors.push_back(new Koopa(this, 2, 4));

// 1. Initialize the data structures used to keep track of your game’s level and actors.
// 2. Load the current level’s details from the appropriate level data file.
// 3. Allocate and insert a valid Player object into the game world.
// 4. Allocate and insert any Kong objects, Floor objects, Ladder objects, Bonfire objects,
// Fireball objects, Koopa objects, Goodie objects, or other relevant objects into the game
// world, as required by the specification in the current level’s data file.
    switch(result) {
        case GWSTATUS_PLAYER_WON: {
            return GWSTATUS_PLAYER_WON;
        }
        case GWSTATUS_LEVEL_ERROR: {
            return GWSTATUS_LEVEL_ERROR;
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    setDisplayText();
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

    if (m_player != nullptr && m_player->isDead()) { // the player pointer will be deleted when we call cleanUp()
        return GWSTATUS_PLAYER_DIED;
    }

    if (getLevelCompletedStatus()) {
        setLevelCompleted(false);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::setLevelCompleted(bool b) {
    m_level_completed = b;
}

bool StudentWorld::getLevelCompletedStatus() const {
    return m_level_completed;
}

void StudentWorld::setDisplayText() {
    int score = getScore();
    int level = getLevel();
    int livesLeft = getLives();
    unsigned int burps = getPlayer()->getNumBurps();
    // Next, create a string from your statistics, of the form:
    // Score: 0000100 Level: 03 Lives: 03 Burps: 08
    // string s = generate_stats(score, level, livesLeft, burps);

    ostringstream oss;
    oss << "Score: ";
    oss.fill('0');
    oss << setw(7) << score;
    
    oss << "  Level: ";
    oss << setw(2) << level;
    // oss.fill('0');
    oss << "  Lives: ";
    oss << setw(2) << livesLeft;
    // oss.fill('0');
    oss << "  Burps: ";
    oss << setw(2) << burps;
    // oss.fill('0');

    string s = oss.str();
    // Finally, update the display text at the top of the screen with your
    // newly created stats
    setGameStatText(s); // calls our provided GameWorld::setGameStatText
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

int StudentWorld::loadLevel(string lvl) {
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(lvl);
    if (result == Level::load_fail_file_not_found) {
        cerr << "Could not find level00.txt data file\n";
        return GWSTATUS_PLAYER_WON;
    }
    else if (result == Level::load_fail_bad_format) {
        cerr << "Your level was improperly formatted\n";
        return GWSTATUS_LEVEL_ERROR;
    }
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
                        Kong * m_kong = new Kong(this, x, y, GraphObject::left);
                        m_actors.push_back(m_kong);
                        break;
                    }
                    case Level::right_kong: {
                        Kong * m_kong = new Kong(this, x, y, GraphObject::right);
                        m_actors.push_back(m_kong);
                        break;
                    }
                    case Level::fireball: {
                        Fireball * m_fireball = new Fireball(this, x, y);
                        m_actors.push_back(m_fireball);
                        break;
                    }
                    case Level::koopa: {
                        cerr << "koopa created at " << x << ", " << y << endl;
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
    return GWSTATUS_CONTINUE_GAME;
}

Actor* StudentWorld::getPtr(int x, int y, Actor* dont_return_me, bool only_return_what_can_die) {
    // what if there are multiple things on the same tile?
    // if (dont_return_me != nullptr) {
    //     cerr << "i am going to skip over me" << endl;
    // }
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors.at(i) != dont_return_me) {
            
            if (m_actors.at(i)->getX() == x && m_actors.at(i)->getY() == y) {
                // if (dont_return_me != nullptr) {
                //     // cerr << "i am at position (" << dont_return_me->getX() << ", " << dont_return_me->getY() << "): found another at the same position as me" << endl;
                // }
                if (only_return_what_can_die && m_actors.at(i)->canDie()) {
                    // cerr << "returning what can die" << endl;
                    return m_actors[i];
                    // continue;
                } 

                if (!only_return_what_can_die) {
                    return m_actors[i];
                }
                
                // if (m_actors[i] != nullptr && m_actors[i] != dont_return_me) {
                    
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

Player * StudentWorld::getPlayer() const {
    return m_player;
}

void StudentWorld::addObject(Actor * ptr) {
    m_actors.push_back(ptr);
}

StudentWorld::~StudentWorld() {
    cleanUp();
}