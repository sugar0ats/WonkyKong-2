#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

Actor::Actor(StudentWorld* sw, int ID, int x, int y)
 : GraphObject(ID, x, y), m_world(sw) // direction is a default parameter that is set to none (when we don't specify a fourth parameter to the GraphObject parameter)
{
}

void Actor::doSomething()
{
}

void Actor::getAttacked(Actor * ptr) {
}

bool Actor::canDie() const {
    return false;
}

bool Actor::isEvil() const {
    return false;
}

bool Actor::isClimbable() const {
    return false;
}

bool Actor::isObstruction() const {
    return false;
}

bool Actor::isDestructive() const {
    return false;
}

bool Actor::isGoodie() const {
    return false;
}

bool Actor::isDead() const {
    return false; 
}

StudentWorld* Actor::getWorld() const { // return a pointer to the student world object, where we can follow the pointer and call various methods
    return m_world;
}

Attacker::Attacker(StudentWorld* sw, int id, int x, int y) : Actor(sw, id, x, y) {

}

bool Attacker::attack() {
    Actor * ptr = getWorld()->getPtr(getX(), getY(), this, true);

    // while there are actors at this location that can die and are what the attacker can attack,
    while (!isDead() && ptr != nullptr && !ptr->isDead() && special_conditions(ptr)) {
        bool doAttack = specialization_attack(ptr); // do specialized behaviors specific to this attacker that can be overriden
        // returns a bool that decides whether the pointer should be attacked
        if (doAttack) {
            ptr->getAttacked(this); // has to know somehow what is attacking it, so pass in itself to the getAttacked() method
        }
        ptr = getWorld()->getPtr(getX(), getY(), this, true); // get the next ptr at this location, if it exists
    }

    return true;
}

bool Attacker::specialization_attack(Actor * other) { // a default attacker does nothing specialized.
    return true;
    
}

bool Attacker::special_conditions(Actor * ptr) {
    return ptr == getWorld()->getPlayer(); // many enemies by default check for the player, this ensures that they only attack the player
}

Floor::Floor(int x, int y) : Actor(nullptr, IID_FLOOR, x, y) { // the floor will never need to modify StudentWorld, so pass in a nullptr instead

}

bool Floor::isObstruction() const { // override the Actor's isObstruction() method to return true so that we can indicate that it is a floor
    return true;
}

Ladder::Ladder(int x, int y) : Actor(nullptr, IID_LADDER, x, y) {

}

bool Ladder::isClimbable() const {
    return true;
}

Bonfire::Bonfire(StudentWorld * sw, int x, int y) : Attacker(sw, IID_BONFIRE, x, y) {

}

bool Bonfire::isDestructive() const {
    return true;
}

void Bonfire::doSomething() {
    increaseAnimationNumber();
    attack();
}

// bonfires can only attack players and barrels
bool Bonfire::special_conditions(Actor * ptr) {
    return Attacker::special_conditions(ptr) || ptr->canRoll();
}

Mortal::Mortal(StudentWorld* sw, int id, int x, int y) : Attacker(sw, id, x, y), m_alive(true) {

}

bool Mortal::canDie() const {
    return true;
}

bool Mortal::isDead() const {
    return !m_alive;
}

// bool Mortal::canMove() const {
//     return false;
// }

void Mortal::setDead(bool dead) {
    m_alive = !dead;
}

Goodie::Goodie(StudentWorld * sw, int id, int x, int y, int points) : Mortal(sw, id, x, y), m_points(points) {

}

bool Goodie::isGoodie() const {
    return true;
}

int Goodie::getPoints() const {
    return m_points;
}
 
void Goodie::doSomething() {
    if (isDead()) {
        return;
    }
    attack();

}

bool Goodie::specialization_attack(Actor * ptr) {
    getWorld()->increaseScore(getPoints()); // goodies should increase the player's score appropriately
    setDead(true); // set itself as dead so it is removed from the list of actors next tick
    getWorld()->playSound(SOUND_GOT_GOODIE); // play the sound a goodie should play

    goodie_special_behavior(); // perform actions specific to each goodie

    return false;
}

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld * sw, int x, int y) : Goodie(sw, IID_EXTRA_LIFE_GOODIE, x, y, 50) {

}

// ExtraLifeGoodies give the player an extra life
void ExtraLifeGoodie::goodie_special_behavior() {
    getWorld()->incLives();
}

GarlicGoodie::GarlicGoodie(StudentWorld * sw, int x, int y) : Goodie(sw, IID_GARLIC_GOODIE, x, y, 25) {

}

// GarlicGoodies give the player 5 burps
void GarlicGoodie::goodie_special_behavior() {
    Player * m_p = getWorld()->getPlayer();
    m_p->giveBurps(5);
}

MovingMortal::MovingMortal(StudentWorld * sw, int id, int x, int y, int direction) : Mortal(sw, id, x, y) {
    setDirection(direction);
}

bool MovingMortal::canMoveThere(int x, int y) {
    // get a pointer to an Actor at this position, as long as it is not the MovingMortal calling canMoveThere.
    Actor * ptr = getWorld()->getPtr(x, y, this);
    if (ptr == nullptr) { // if there is nothing at that position, the MovingMortal can move there
        return true;
    }
    // if there is a floor at that position, then return false.
    if (getWorld()->getPtr(x, y)->isObstruction()) {
        return false;
    } else {
        return true;
    }
}

bool MovingMortal::canFallThere(int x, int y) {
    return canMoveThere(x, y) && !canClimbThere(x, y);
    // a MovingMortal can fall at a position if there is no floor and no ladder at that position
}

bool MovingMortal::canClimbThere(int x, int y) {
    Actor * ptr = getWorld()->getPtr(x, y, this);
    // if there is no Actor other than this one at a specific position or there is no ladder, then the player can't climb at this position.
    if (ptr == nullptr || !ptr->isClimbable()) {
        return false;
    } 
    return true;
    
}

Burp::Burp(StudentWorld * sw, int x, int y, int dir) : MovingMortal(sw, IID_BURP, x, y, dir), m_lifetime(5) {

}

void Burp::doSomething() {
    if (m_lifetime <= 0) {
        setDead(true);
        return;
    } else {
        // decrement the burp's lifetime as long as its alive
        m_lifetime--;
        attack(); // attack whatever enemy is on the same square as it, if there is one
    }
}

// Burps will only attack Enemies (like Koopas, Fireballs, Barrels)
bool Burp::special_conditions(Actor * ptr) {
    return ptr->isEvil();
}

Player::Player(StudentWorld* sw, int x, int y) : MovingMortal(sw, IID_PLAYER, x, y, right), m_burps(0), m_freezecount(0), m_jumpcount(0) {
    // cout << "player has " << getWorld()->getLives() << " lives" << endl;
}

void Player::setFreezeCount(int count) {
    m_freezecount = count;
}

void Player::giveBurps(int n) {
    m_burps = n;
}

int Player::getNumBurps() const {
    return m_burps;
}

// by default, when the player is attacked,
void Player::getAttacked(Actor * ptr) {
    getWorld()->playSound(SOUND_PLAYER_DIE); // play the correct sound
    getWorld()->decLives(); // decrement lives
    setDead(true); // set the player as dead
}

void Player::doSomething() {
    if (isDead()) {
        return;
    }

    // if the player is frozen, do nothing for this tick
    if (m_freezecount > 0) {
        m_freezecount--;
        return;
    }

    // create a 2d vector indicating the direction the player is to travel in
    vector<int> direction_to_travel_in(2);

    // if the player is in the middle of jumping,
    if (m_jumpcount > 0) {
        int dir = getDirection() == left ? -1 : 1; // get the direction the player is jumping
        switch(m_jumpcount) {
            // for the middle three frames of the jump,
            case 4:
            case 3:
            case 2:
                if (canMoveThere(getX() + dir, getY()) && !canClimbThere(getX(), getY())) { // if the player can move to the next position and there is no ladder at that position, continue to travel in that direction
                    direction_to_travel_in[0] = dir; // set the x coord of the direction to move in to be the previous direction
                } else {
                    m_jumpcount = 0; // terminate the jump early
                }
                break;
    
            case 1:
                if (canMoveThere(getX(), getY() - 1) && !canClimbThere(getX(), getY())) { // if the player can fall down to the tile below it, 
                    direction_to_travel_in[1] = -1; // change the direction to move in to reflect falling
                } else {
                    m_jumpcount = 0; // terminate the jump early
                }
                break;
    
            default:
                break;
        }

        m_jumpcount--;
    } else if (canFallThere(getX(), getY() - 1) && !canClimbThere(getX(), getY())) { // if space below is empty
        direction_to_travel_in[1] = -1; // change the direction to move in to reflect falling
    } else {
        int ch;
        if (getWorld()->getKey(ch))
        {
        // user hit a key this tick!
            switch (ch)
            {
                case KEY_PRESS_DOWN: {
                    // if there is a ladder below the player OR the player is on a ladder AND there is free space below the player, the player can move down
                    if (canClimbThere(getX(), getY() - 1) || (canClimbThere(getX(), getY()) && canMoveThere(getX(), getY() - 1))) {
                        direction_to_travel_in[1] = -1;
                    }
                    break;
                }
                case KEY_PRESS_UP: {
                    // if the player is on a ladder and the tile above the ladder is not an obstruction, the player can move up
                    if (canClimbThere(getX(), getY()) && canMoveThere(getX(), getY() + 1)) {
                        direction_to_travel_in[1] = 1;
                    }
                    break;
                }
                case KEY_PRESS_LEFT: {
                    direction_to_travel_in[0] = moveLaterally(left);
                    break;
                }
                
                case KEY_PRESS_RIGHT: {
                    direction_to_travel_in[0] = moveLaterally(right);
                    break;
                }
                
                case KEY_PRESS_SPACE: {
                    direction_to_travel_in[1] = startJump();
                    break;
                }
                case KEY_PRESS_TAB: {
                    addBurp();
                    break;
                }
                
            }
        
        }
    }
    
    // if there is a direction to move to, move in that direction!
    if (direction_to_travel_in[0] != 0 || direction_to_travel_in[1] != 0) {
        moveTo(getX() + direction_to_travel_in[0], getY() + direction_to_travel_in[1]);
    }
}

int Player::startJump() {
    // initiate a jump sequence!
    getWorld()->playSound(SOUND_JUMP);
    // if we aren't already jumping:
    if (m_jumpcount <= 0) {
        m_jumpcount = 4;
        // do the first tick of the jump, which is to check if we can move one up. move one up if we can.
        if (canMoveThere(getX(), getY() + 1)) {
            return 1;
        } else {
            m_jumpcount = 0; // terminate the jump early
            
        }
    }
    return 0;
}

void Player::addBurp() {
    // if we have burps,
    if (m_burps > 0) {
        // create a new burp in the direction the player is standing
        Burp * m_burp = new Burp(getWorld(), getX() + (getDirection() == left ? -1 : 1), getY(), getDirection());
        // play the corresponding sounds and add the Burp to the list of actors in StudentWorld
        getWorld()->playSound(SOUND_BURP);
        getWorld()->addObject(m_burp);
        m_burps--;
        // decrement the number of burps after using one
    }
}

int Player::moveLaterally(int dir) {
    int d = (dir == right ? 1 : -1);

    // if the direction we want to go in isn't the direction we currently face,
    if (getDirection() != dir) { 
        setDirection(dir); // set it to the player's direction and return.
        return 0;
    }
    // if we can't move in the desired direction,
    if (!canMoveThere(getX() + d, getY())) {
        return 0; // return early.
    }
    return d; // return the value needed to move the player 1 tile in that direction
}

Enemy::Enemy(StudentWorld* sw, int id, int x, int y, int dir) : MovingMortal(sw, id, x, y, dir), m_delay(0) {

}

bool Enemy::isEvil() const {
    return true;
}

void Enemy::specialized_enemy_actions() {

}

void Enemy::getAttacked(Actor * ptr) {
    setDead(true);
    getWorld()->playSound(SOUND_ENEMY_DIE);
    getWorld()->increaseScore(100);
    special_death_actions(); // when an enemy dies, it might have some special behavior that it does 
}

// each tick, an Enemy checks if it has died,
void Enemy::doSomething() {
    if (isDead()) {
        return;
    }

    attack(); // it must also attack

    specialized_enemy_actions(); // and perform specialized actions

}

// for cooldown actions, that is, actions that occur every n ticks. they can be generalized with this function that all Enemys inherit
bool Enemy::timeToDoAction(int n) {
    if (m_delay >= n) {
        m_delay = 0;
        return true;
    } else {
        m_delay++;
        return false;
    }
}

Kong::Kong(StudentWorld* sw, int x, int y, int dir) : Enemy(sw, IID_KONG, x, y, dir), m_flee_state(false), m_time_since_last_barrel(0), m_barrel_throw_period(max(200 - 50 * getWorld()->getLevel(), 50)) {

}

void Kong::setFlee(bool value) {
    m_flee_state = value;
}

bool Kong::isFleeing() const {
    return m_flee_state;
}

void Kong::specialized_enemy_actions() {
    increaseAnimationNumber();

    // if the Kong is within 2 tiles of the player, it must begin fleeing
    if (euclidian_dist_w_player() <= 2) {
        setFlee(true);
    }

    // if it's time to throw a barrel, create a new Barrel object in the direction the Kong is facing and add it to StudentWorld
    if (!isFleeing() && m_time_since_last_barrel >= m_barrel_throw_period) {
        m_time_since_last_barrel = 0;
        int dir = getDirection() == left ? -1 : 1;
        Barrel * m_barrel = new Barrel(getWorld(), getX() + dir, getY(), getDirection());
        getWorld()->addObject(m_barrel);
    }

    // every 5 ticks, 
    if (timeToDoAction(5)) {
        if (isFleeing()) { // if Kong is fleeing, move Kong up 1 tile
            moveTo(getX(), getY() + 1);
            if (getY() >= VIEW_HEIGHT) { // when Kong gets to the top of the scren, we know the level has been completed
                getWorld()->increaseScore(1000);
                getWorld()->playSound(SOUND_FINISHED_LEVEL);
                getWorld()->setLevelCompleted(true);
            }
        }
    }
    // increment the time since the last barrel throw
    m_time_since_last_barrel++;
}

int Kong::euclidian_dist_w_player() {
    Actor * m_p = getWorld()->getPlayer(); // get a pointer pointing to the player
    return sqrt(pow(m_p->getX() - getX(), 2) + pow(m_p->getY() - getY(), 2)); // return the distance between the Kong and the player
}

void Kong::getAttacked(Actor * ptr) {
    // do nothing when attacked, because nothing is meant to interact with it
}


Koopa::Koopa(StudentWorld* sw, int x, int y) : Enemy(sw, IID_KOOPA, x, y, randInt(0, 1) == 0 ? left : right), m_freeze_cooldown(0), m_just_froze_something(false) {

}

void Koopa::specialized_enemy_actions() {
    // if the cooldown is greater than 0, decrement until it is at 0
    if (m_freeze_cooldown > 0) {
        m_freeze_cooldown--;
    }

    // get the direction Koopa is facing in
    int dir = (getDirection() == left) ? -1 : 1;
    if (!m_just_froze_something && timeToDoAction(10)) { // if Koopa hasn't just frozen something and its the 10th tick,
        bool alreadyMoved = false;
        // if there is a wall in front of Koopa or the Koopa would fall if it stepped there, reverse its direction
        if (!canMoveThere(getX() + dir, getY()) || canFallThere(getX() + dir, getY() - 1)) {
            setDirection(dir == 1 ? left : right); // reverse direction of fireball
            dir = getDirection() == left ? -1 : 1;
            alreadyMoved = true;
            
        } 

        // if not, then the Koopa can continue moving in its current direction
        if (!alreadyMoved) {
                moveTo(getX() + dir, getY());
        }

        attack();
    } 

    m_just_froze_something = false;
}

// Koopa will only attack the player if its freeze cooldown is 0
bool Koopa::special_conditions(Actor * ptr) {
    return m_freeze_cooldown <= 0 && Attacker::special_conditions(ptr);
}

bool Koopa::specialization_attack(Actor * ptr) {
    m_freeze_cooldown = 50; // set its freeze cooldown so it can't repeatedly freeze the player
    m_just_froze_something = true;

    // access just the player in particular
    Player * m_p = getWorld()->getPlayer();
    m_p->setFreezeCount(50);
    
    return false;
}

void Koopa::special_death_actions() {
    // Koopa has a one in three chance of dropping an ExtraLifeGoodie
    if (randInt(0, 2) == 0) {
        ExtraLifeGoodie * m_extra = new ExtraLifeGoodie(getWorld(), getX(), getY());
        getWorld()->addObject(m_extra);
    }
}

Fireball::Fireball(StudentWorld* sw, int x, int y) : Enemy(sw, IID_FIREBALL, x, y, randInt(0, 1) == 0 ? left : right), m_climbing_state(0) {

}

void Fireball::specialized_enemy_actions() {
    // every 10 ticks, the Fireball moves:
    if (timeToDoAction(10)) {
        // get the direction the Fireball is currently facing
        int dir = (getDirection() == left) ? -1 : 1;
        bool alreadyMoved = false;
        // if the Fireball is not currently climbing down, is on a ladder and the tile above it is free OR there's a ladder on the tile above it
        if (canClimbThere(getX(), getY()) && (canMoveThere(getX(), getY()+1) || canClimbThere(getX(), getY() + 1)) && !isClimbingDown()) {

            // the Fireball has a one in three chance of beginning to climb up if it isn't already climbing up
            if (isClimbingUp() || randInt(0, 2) == 0) {
                setClimbingState(1);
                moveTo(getX(), getY() + 1); // move the fireball up one
                alreadyMoved = true; // flag that the fireball has moved already this tick
            }
            
        } else if (canClimbThere(getX(), getY() - 1) && !isClimbingUp()) {
            // if the Fireball is not climbing up and it can climb down,
            // it has a one in three chance of climbing down if it hasn't already
            if (isClimbingDown() || randInt(0, 2) == 0) {
                setClimbingState(-1);
                moveTo(getX(), getY() - 1); // move the fireball down one
                alreadyMoved = true;
            }
            
        } 

        // assuming the Fireball hasn't already moved this tick,
        if (!alreadyMoved) {
            // if the Fireball is climbing, it should continue climbing in the desired direction
            if (isClimbing() && (!canClimbThere(getX(), getY() + getClimbingState()) || !canMoveThere(getX(), getY() + getClimbingState()))) {
                setClimbingState(0);
            }
            // if the Fireball can't move in the direction it's facing in, or it would fall if it moved in the direction it's facing, reverse its direction
            if (!canMoveThere(getX() + dir, getY()) || canFallThere(getX() + dir, getY() - 1)) {
                setDirection(dir == 1 ? left : right); // reverse direction of fireball
                dir = getDirection() == left ? -1 : 1;
                alreadyMoved = true;
                
            } 

        }

        // if the Fireball hasn't already moved, move in the direction it's currently facing in
        if (!alreadyMoved) {
            moveTo(getX() + dir, getY());
            
        }
        attack();
    }
}

bool Fireball::isClimbingUp() const {
    return m_climbing_state == 1;
}

bool Fireball::isClimbingDown() const {
    return m_climbing_state == -1;
}

bool Fireball::isClimbing() const {
    return isClimbingUp() || isClimbingDown();
}

void Fireball::setClimbingState(int i) {
    m_climbing_state = i;
}

int Fireball::getClimbingState() const {
    return m_climbing_state;
}

void Fireball::special_death_actions() {
    // the Fireball has a 1 in 3 chance of dropping a Garlic Goodie
    if (randInt(0, 2) == 0) {
        GarlicGoodie* m_garlic = new GarlicGoodie(getWorld(), getX(), getY());
        getWorld()->addObject(m_garlic);
    }
}

Barrel::Barrel(StudentWorld * sw, int x, int y, int dir) : Enemy(sw, IID_BARREL, x, y, dir) {

}

void Barrel::specialized_enemy_actions() {
    // if there is an empty space or a ladder below the Barrel, have the Barrel move down one
    if(canFallThere(getX(), getY() - 1) || canClimbThere(getX(), getY() - 1)) {
        moveTo(getX(), getY() - 1);

        // if there's a floor beneath it, reverse its direction
        if(!canMoveThere(getX(), getY() - 1)) {
            setDirection(getDirection() == left ? right : left);
        }
    }

    // every 10 ticks, 
    if(timeToDoAction(10)) {
        int dir = getDirection() == left ? -1 : 1;
        // if the Barrel is blocked in the direction it's trying to move in, reverse its direction
        if (!canMoveThere(getX() + dir, getY())) {
            setDirection(getDirection() == left ? right : left);
        } else {
            // otherwise, have the Barrel continue to move in the direction it was already going in
            moveTo(getX() + dir, getY());
        }
    }
}


void Barrel::getAttacked(Actor * ptr) {
    // if a Burp destroyed the Barrel and not a Bonfire, it should play a sound and increase the Player's score
    if (!ptr->isDestructive()) {
        getWorld()->increaseScore(100);
        getWorld()->playSound(SOUND_ENEMY_DIE);
    } 
    setDead(true);
    return;
    
}
