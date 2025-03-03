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
    // initialize level completed as false, because the level has not been completed
}

int StudentWorld::init()
{
    if (getLevel() == 100) { // once the player finishes the 99th level and the level becomes 100, indicate that the player has won
        return GWSTATUS_PLAYER_WON;
    }
    string str = "level"; // create string representing level file
    if (getLevel() < 10) {
        str += '0';
        
    } 
    str += (getLevel() + '0');
    str += ".txt";
    int result = loadLevel(str); // loads the file and adds all dynamically allocated objects to the private vector
    // result stores what loadLevel returns, which we use to decide what to return from this function

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
                delete m_actors.at(i); // deallocate the actor
                m_actors.at(i) = nullptr; // set the pointer to a nullptr
                m_actors.erase(m_actors.begin() + i); // erase the actor from the vector
            }
        }
    }

    if (getLevelCompletedStatus()) { // if we've completed the level, 
        setLevelCompleted(false); // reset the member variable that indicates that we've finished a level
        return GWSTATUS_FINISHED_LEVEL; // return the corresponding value to tell the framework to proceed as necessary
    }

    if (m_player != nullptr && m_player->isDead()) { // the player pointer will be deleted when we call cleanUp()
        return GWSTATUS_PLAYER_DIED;
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

    // use stringstreams to ensure all text is properly formatted even when it changes
    ostringstream oss;
    oss << "Score: ";
    oss.fill('0');
    oss << setw(7) << score;
    
    oss << "  Level: ";
    oss << setw(2) << level;
    oss << "  Lives: ";
    oss << setw(2) << livesLeft;
    oss << "  Burps: ";
    oss << setw(2) << burps;

    string s = oss.str();
    setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

void StudentWorld::cleanUp()
{
    if (m_player != nullptr) {
        delete m_player; // deallocate the actor
        m_player = nullptr; // set the pointer to a nullptr so we don't try to delete it again when we call the destructor
    }
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors.at(i) != nullptr) { // as long as the actor in the list is not a nullptr (as in, we haven't cleaned up before)
            delete m_actors.at(i); // deallocate the actor
            m_actors.at(i) = nullptr; // set the actor's pointer to be a nullptr
        }
        // remember that you can't delete a nullptr!
    }

    m_actors.clear(); // empty the list of actors
}

int StudentWorld::loadLevel(string lvl) {

    //create a Level object with the desired assetPath
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(lvl);
    // if the result results in an improper level, return the corresponding error code
    if (result == Level::load_fail_file_not_found) {
        return GWSTATUS_PLAYER_WON;
    }
    else if (result == Level::load_fail_bad_format) {
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success)
        {
            // for each tile in the text file, check what type of character it is and add the corresponding Actor to the list of Actors
        for (int x = 0; x < VIEW_HEIGHT; x++) {
            for (int y = 0; y < VIEW_WIDTH; y++) {
                Level::MazeEntry item = lev.getContentsOf(x, y);

                switch (item) {
                    case Level::floor: {
                        Floor* m_floor = new Floor(x, y);
                        m_actors.push_back(m_floor);
                        break;
                    }
                    case Level::ladder: {
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
                        Koopa * m_koopa = new Koopa(this, x, y);
                        m_actors.push_back(m_koopa);
                        break;
                    }
                    case Level::bonfire: {
                        Bonfire * m_bonfire = new Bonfire(this, x, y);
                        m_actors.push_back(m_bonfire);
                        break;
                    }
                        
                    case Level::extra_life: {
                        ExtraLifeGoodie * m_extra_life = new ExtraLifeGoodie(this, x, y);
                        m_actors.push_back(m_extra_life);
                        break;
                    }
                    case Level::garlic: {
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
    // iterate through every actor
    for (int i = 0; i < m_actors.size(); i++) {
        // if specified, the method will skip over the Actor that the pointer that is passed in points to. this is so that an Actor, when trying to get the pointer of another Actor at the same location, doesn't get a pointer to itself.
        if (m_actors.at(i) != dont_return_me) {
            
            // if specified, the method will only return Actors at the specified location that can die.
            if (m_actors.at(i)->getX() == x && m_actors.at(i)->getY() == y) {
                if (only_return_what_can_die && m_actors.at(i)->canDie()) {
                    return m_actors[i];
                } 

                if (!only_return_what_can_die) {
                    return m_actors[i];
                }
                
            }
        }
        
    }

    // if not found in the Actor list, return the Player if it's at the right location (that is, if the Player is not the one calling this method)
    if (m_player != dont_return_me && m_player->getX() == x && m_player->getY() == y) {
        return m_player;
    }

    // if nothing fulfills the requirements specified at this location, return a nullptr
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