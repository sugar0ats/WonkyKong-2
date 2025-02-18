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

StudentWorld* Actor::getWorld() { // return a pointer to the student world object, where we can follow the pointer and call various methods
    return m_world;
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

Bonfire::Bonfire(StudentWorld * sw, int x, int y) : Actor(sw, IID_BONFIRE, x, y) {

}

bool Bonfire::isDestructive() const {
    return true;
}

Mortal::Mortal(StudentWorld* sw, int id, int x, int y) : Actor(sw, id, x, y), m_alive(true) {

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

Player::Player(StudentWorld* sw, int x, int y) : MovingMortal(sw, IID_PLAYER, x, y, right) {
}

void Player::setFreezeCount(int count) {
    m_freezecount = count;
}

void Player::doSomething() {
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
                if (canMoveThere(getX() + dir, getY())) {
                    direction_to_travel_in[0] = (dir);
                } else {
                    m_jumpcount = 0;
                }
                break;
    
            case 1:
                if (canMoveThere(getX(), getY() - 1)) {
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