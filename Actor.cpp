#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

Actor::Actor(StudentWorld* sw, int ID, int x, int y)
 : GraphObject(ID, x, y), m_world(sw) // direction is a default parameter that is set to none (when we don't specify a fourth parameter to the GraphObject parameter)
{
}

void Actor::doSomething()
{
    // stub
}

void Actor::getAttacked() {
    // stub
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
    return false; // STUB
}

StudentWorld* Actor::getWorld() const { // return a pointer to the student world object, where we can follow the pointer and call various methods
    return m_world;
}

Attacker::Attacker(StudentWorld* sw, int id, int x, int y) : Actor(sw, id, x, y) {

}

bool Attacker::attack() {
    // check if there's anything on the same square as it
    // cerr << "attacker's pos is (" << getX() << ", " << getY() << ")" << endl;
    Actor * ptr = getWorld()->getPtr(getX(), getY(), this);
    // cerr << "i am checking for myself" << endl;
    // if (ptr != nullptr) {
    //     cerr << "can this pointer die? " << ptr->canDie() << endl;
    // }
    
    if (ptr != nullptr && ptr->canDie() && special_conditions(ptr)){
        cerr << "found something...or someone...else!" << endl;
        specialization_attack(ptr); // do something to that other! (like freeze it or something, if it can be frozen)
        ptr->getAttacked(); // has to know somehow what is attacking it
        
        return true;
    }
    // cerr << "didn't find anything..." << endl;

    return false;
}

void Attacker::specialization_attack(Actor * other) {
    
}

bool Attacker::special_conditions(Actor * ptr) {
    return true;
}

Floor::Floor(int x, int y) : Actor(nullptr, IID_FLOOR, x, y) {

}

bool Floor::isObstruction() const {
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

bool Bonfire::special_conditions(Actor * ptr) {
    return !ptr->isEvil();
}

void Bonfire::doSomething() {
    increaseAnimationNumber();

    attack();
}

Mortal::Mortal(StudentWorld* sw, int id, int x, int y) : Attacker(sw, id, x, y), m_alive(true) {

}

bool Mortal::canDie() const {
    return true;
}

bool Mortal::isDead() const {
    return !m_alive;
}

bool Mortal::canMove() const {
    return false;
}

void Mortal::setDead(bool dead) {
    m_alive = !dead;
}

Goodie::Goodie(StudentWorld * sw, int id, int x, int y) : Mortal(sw, id, x, y) {

}

bool Goodie::isGoodie() const {
    return true;
}

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld * sw, int x, int y) : Goodie(sw, IID_EXTRA_LIFE_GOODIE, x, y) {

}

GarlicGoodie::GarlicGoodie(StudentWorld * sw, int x, int y) : Goodie(sw, IID_GARLIC_GOODIE, x, y) {

}

MovingMortal::MovingMortal(StudentWorld * sw, int id, int x, int y, int direction) : Mortal(sw, id, x, y) {
    setDirection(direction);
}

bool MovingMortal::canMoveThere(int x, int y) {
    Actor * ptr = getWorld()->getPtr(x, y);
    if (ptr == nullptr) {
        return true;
    }
    if (getWorld()->getPtr(x, y)->isObstruction()) {
        // cerr << "found a non empty space and its a wall. we can't move!" << endl;
        return false;
    } else {
        // cerr << "found a non empty space and its not a wall. we can move!" << endl;
        return true;
    }
    // return !(getWorld()->getPtr(x, y)->isObstruction());
}

bool MovingMortal::canFallThere(int x, int y) {
    return canMoveThere(x, y) && !canClimbThere(x, y);
}

bool MovingMortal::canClimbThere(int x, int y) {
    Actor * ptr = getWorld()->getPtr(x, y);
    if (ptr == nullptr || !ptr->isClimbable()) {
        return false;
    } 
    return true;
    
}

bool MovingMortal::canMove() const {
    return true;
}

Burp::Burp(StudentWorld * sw, int x, int y, int dir) : MovingMortal(sw, IID_BURP, x, y, dir), m_lifetime(5) {

}

void Burp::doSomething() {
    if (m_lifetime <= 0) {
        setDead(true);
        return;
    } else {
        m_lifetime--;
        // attack enemy on nearby square

        // class type Attacker
        // method attack() 
        // getWorld()->getPtrAt(..., ...)->getAttacked()
    }
}



Player::Player(StudentWorld* sw, int x, int y) : MovingMortal(sw, IID_PLAYER, x, y, right), m_lives(getWorld()->getLives()) {
    cout << "player has " << getWorld()->getLives() << " lives" << endl;
}

void Player::setFreezeCount(int count) {
    m_freezecount = count;
}

void Player::getAttacked() {
    if (m_freezecount > 0) { // so we don't lose a life when frozen
        return;
    }
    
    getWorld()->playSound(SOUND_PLAYER_DIE);
    getWorld()->decLives();
    // getWorld()->isGameOver();
    setDead(true);
    cerr << "player now has " << getWorld()->getLives() << endl;
}


void Player::doSomething() {
    // m_dead = (getWorld()->getLives)
    // cerr << "player's pos is (" << getX() << ", " << getY() << ")" << endl;

    if (isDead()) {
        return;
    }

    if (m_freezecount > 0) {
        m_freezecount--;
        return;
    }

    vector<int> direction_to_travel_in(2);

    if (m_jumpcount > 0) {
        int dir = getDirection() == left ? -1 : 1;
        switch(m_jumpcount) {
            case 4:
            case 3:
            case 2:
                if (canMoveThere(getX() + dir, getY()) && !canClimbThere(getX(), getY())) {
                    direction_to_travel_in[0] = dir;
                } else {
                    m_jumpcount = 0;
                }
                break;
    
            case 1:
                if (canMoveThere(getX(), getY() - 1) && !canClimbThere(getX(), getY())) {
                    direction_to_travel_in[1] = -1;
                } else {
                    m_jumpcount = 0;
                }
                break;
    
            default:
                break;
        }

        m_jumpcount--;
    } else if (canFallThere(getX(), getY() - 1)) { // if space below is empty
        direction_to_travel_in[1] = -1;
    } else {
        int ch;
        if (getWorld()->getKey(ch))
        {
        // user hit a key this tick!
            
            switch (ch)
            {
                case KEY_PRESS_DOWN: {
                    if (canClimbThere(getX(), getY() - 1)) {
                        direction_to_travel_in[1] = -1;
                    }
                    break;
                }
                case KEY_PRESS_UP: {
                    if (canClimbThere(getX(), getY())) {
                        direction_to_travel_in[1] = 1;
                    }
                    break;
                }
                case KEY_PRESS_LEFT: {
                    //... turn or move left ...
                    // if current direction isn't left, set it to left and break!
                    if (getDirection() != left) { 
                        setDirection(left);
                        break;
                    }
                    // cout << "check if i can move to " << getX() - 1 << " " << getY() << endl;
                    if (!canMoveThere(getX() - 1, getY())) {
                        break;
                    }
                    // moveTo(getX() - 1, getY());
                    direction_to_travel_in[0] = -1;
                    // cout << "left!" << endl;
                    // if tile to left is a wall, break
                    // else set location to be c-1, r
                    break;
                }
                
                case KEY_PRESS_RIGHT: {
                    //... turn or move right ...
                    // if current direction isn't right, set it to right and break!
                    // if tile to the right is a wall, break
                    // else set location to c+1, r
                    if (getDirection() != right) { 
                        setDirection(right);
                        break;
                    }
                    // cout << "check if i can move to " << getX() + 1 << " " << getY() << endl;
                    if (!canMoveThere(getX() + 1, getY())) {
                        break;
                    }
                    // moveTo(getX() + 1, getY());
                    direction_to_travel_in[0] = 1;
                    // cout << "right!" << endl;
                    break;
                }
                
                case KEY_PRESS_SPACE: {
                    if (m_jumpcount <= 0) {
                        m_jumpcount = 4;
                        if (canMoveThere(getX(), getY() + 1)) {
                            direction_to_travel_in[1] = 1;
                        } else {
                            m_jumpcount = 0;
                        }
                    }
                    break;
                }
                case KEY_PRESS_TAB: {
                    //... burp if you have burps left ...
                    if (m_burps > 0) {
                        Burp * m_burp = new Burp(getWorld(), getX() + (getDirection() == left ? -1 : 1), getY(), getDirection());
                        getWorld()->addObject(m_burp);
                        cerr << "new burp created!" << endl;
                        m_burp--;
                    }
                    break;
                    // etc…
                }
                
            }
        
        }
    }
    
    if (direction_to_travel_in[0] != 0 || direction_to_travel_in[1] != 0) {
        moveTo(getX() + direction_to_travel_in[0], getY() + direction_to_travel_in[1]);
    }
}

Enemy::Enemy(StudentWorld* sw, int id, int x, int y, int dir) : MovingMortal(sw, id, x, y, dir) {

}

bool Enemy::isEvil() const {
    return true;
}

Kong::Kong(StudentWorld* sw, int x, int y, int dir) : Enemy(sw, IID_KONG, x, y, dir) {

}

Koopa::Koopa(StudentWorld* sw, int x, int y) : Enemy(sw, IID_KOOPA, x, y, randInt(0, 1) == 0 ? left : right) {
    // randomly set direction

}

Fireball::Fireball(StudentWorld* sw, int x, int y) : Enemy(sw, IID_FIREBALL, x, y, randInt(0, 1) == 0 ? left : right) {

}

Barrel::Barrel(StudentWorld * sw, int x, int y, int dir) : Enemy(sw, IID_BARREL, x, y, dir) {

}