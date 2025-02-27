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

void Actor::getAttacked(Actor * ptr) {
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
    Actor * ptr = getWorld()->getPtr(getX(), getY(), this, true);

    while (!isDead() && ptr != nullptr && !ptr->isDead() && special_conditions(ptr)) {
        specialization_attack(ptr); // do something to that other! (like freeze it or something, if it can be frozen)
        ptr->getAttacked(this); // has to know somehow what is attacking it
        // cerr << "got here" << endl;
        // return true;
        ptr = getWorld()->getPtr(getX(), getY(), this, true);
    }

    // if (ptr != nullptr && special_conditions(ptr)) {
    // }
    
    // cerr << "i am checking for myself" << endl;
    // if (ptr != nullptr) {
    //     cerr << "can this pointer die? " << ptr->canDie() << endl;
    // }
    
    // if (ptr != nullptr && ptr->canDie() && special_conditions(ptr)){
    //     // cerr << "found something...or someone...else!" << endl;
    //     specialization_attack(ptr); // do something to that other! (like freeze it or something, if it can be frozen)
    //     ptr->getAttacked(this); // has to know somehow what is attacking it
    //     // cerr << "got here" << endl;
    //     return true;
    // }
    // cerr << "didn't find anything..." << endl;
    

    return true;
}

void Attacker::specialization_attack(Actor * other) {
    
}

bool Attacker::special_conditions(Actor * ptr) {
    return ptr == getWorld()->getPlayer(); // many enemies by default check for the player.
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

// bool Bonfire::special_conditions(Actor * ptr) {
//     return !ptr->isEvil();
// }

void Bonfire::doSomething() {
    increaseAnimationNumber();

    attack();
}

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

bool Mortal::canMove() const {
    return false;
}

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

// bool Goodie::special_conditions(Actor * ptr) {
//     return ptr->canDie() && !ptr->isEvil();
// }
 
void Goodie::doSomething() {
    if (isDead()) {
        return;
    }
    attack();
}

void Goodie::specialization_attack(Actor * ptr) {
    getWorld()->increaseScore(getPoints());
    setDead(true);
    getWorld()->playSound(SOUND_GOT_GOODIE);
}


ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld * sw, int x, int y) : Goodie(sw, IID_EXTRA_LIFE_GOODIE, x, y, 50) {

}

// void ExtraLifeGoodie::doSomething() {
//     if (isDead()) {
//         return;
//     }
    
//     attack();
// }

// void ExtraLifeGoodie::specialization_attack() {
//     getWorld()->increaseScore(getPoints());
//     isDead();
// }

GarlicGoodie::GarlicGoodie(StudentWorld * sw, int x, int y) : Goodie(sw, IID_GARLIC_GOODIE, x, y, 25) {

}

// void GarlicGoodie::specialization_attack(Actor * other) {
//     getWorld()->increaseScore(25);
//     setDead(true);
//     getWorld()->playSound(SOUND_GOT_GOODIE);
//     cerr << "found the player, removing myself (i am garlic)" << endl;
//     // give the player 5 additional burps
//     // when we attack it
// }

MovingMortal::MovingMortal(StudentWorld * sw, int id, int x, int y, int direction) : Mortal(sw, id, x, y) {
    setDirection(direction);
}

bool MovingMortal::canMoveThere(int x, int y) {
    Actor * ptr = getWorld()->getPtr(x, y, this);
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
    // not a floor and not a ladder
}

bool MovingMortal::canClimbThere(int x, int y) {
    Actor * ptr = getWorld()->getPtr(x, y, this);
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
        // cerr << "i'm dead" << endl;
        setDead(true);
        return;
    } else {
        // cerr << "i'm alive!" << endl;
        m_lifetime--;
        attack();
        // attack enemy on nearby square

        // class type Attacker
        // method attack() 
        // getWorld()->getPtrAt(..., ...)->getAttacked()
    }
}

bool Burp::special_conditions(Actor * ptr) {
    return ptr->isEvil() || ptr->canRoll();
}

Player::Player(StudentWorld* sw, int x, int y) : MovingMortal(sw, IID_PLAYER, x, y, right), m_lives(getWorld()->getLives()), m_burps(0), m_freezecount(0), m_jumpcount(0) {
    cout << "player has " << getWorld()->getLives() << " lives" << endl;
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

void Player::getAttacked(Actor * ptr) {
    // if (m_freezecount > 0) { // so we don't lose a life when frozen
    //     return;
    // }
    if (ptr->canFreezeOthers()) {
        cerr << "frozen!" << endl;
        setFreezeCount(50);
        return;
    }
    if (ptr->isSmelly()) {
        cerr << "give the player 5 burps!" << endl;
        giveBurps(5);
        return;
    }
    if (ptr->isExtra()) {
        getWorld()->incLives();
        cerr << "player now has " << getWorld()->getLives() << endl;
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
    } else if (canFallThere(getX(), getY() - 1) && !canClimbThere(getX(), getY())) { // if space below is empty
        direction_to_travel_in[1] = -1;
    } else {
        int ch;
        if (getWorld()->getKey(ch))
        {
        // user hit a key this tick!
            
            switch (ch)
            {
                case KEY_PRESS_DOWN: {
                    if (canClimbThere(getX(), getY() - 1) || (canClimbThere(getX(), getY()) && canMoveThere(getX(), getY() - 1))) {
                        direction_to_travel_in[1] = -1;
                    }
                    break;
                }
                case KEY_PRESS_UP: {
                    if (canClimbThere(getX(), getY()) && canMoveThere(getX(), getY() + 1)) {
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
                    getWorld()->playSound(SOUND_JUMP);
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
                    cout << "you have " << m_burps << " left" << endl;
                    if (m_burps > 0) {
                        Burp * m_burp = new Burp(getWorld(), getX() + (getDirection() == left ? -1 : 1), getY(), getDirection());
                        getWorld()->playSound(SOUND_BURP);
                        getWorld()->addObject(m_burp);
                        cerr << "new burp created!" << endl;
                        m_burps--;
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

void Player::setFrozenTicks(int n) {
    m_freezecount = n;
}

Enemy::Enemy(StudentWorld* sw, int id, int x, int y, int dir) : MovingMortal(sw, id, x, y, dir), m_delay(0) {

}

bool Enemy::isEvil() const {
    return true;
}

void Enemy::getAttacked(Actor * ptr) {
    setDead(true);
    getWorld()->playSound(SOUND_ENEMY_DIE);
    getWorld()->increaseScore(100);
    special_death_actions();
}

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

    // m_barrel_throw_period = 30; // DEBUGGING

}

void Kong::setFlee(bool value) {
    m_flee_state = value;
}

bool Kong::isFleeing() const {
    return m_flee_state;
}

// void Kong::setLevelCompleted(bool b) {
//     m_level_completed = b;
// }

// bool Kong::isLevelCompleted() const {
//     return m_level_completed;
// }

void Kong::doSomething() {
    // cerr << "facing " << (getDirection() == left ? "left" : "right") << endl;
    if (isDead()) {
        return;
    }

    increaseAnimationNumber();

    if (euclidian_dist_w_player() <= 2) {
        setFlee(true);
    }

    if (m_time_since_last_barrel >= m_barrel_throw_period) {
        m_time_since_last_barrel = 0;
        int dir = getDirection() == left ? -1 : 1;
        Barrel * m_barrel = new Barrel(getWorld(), getX() + dir, getY(), getDirection());
        getWorld()->addObject(m_barrel);
    }

    if (timeToDoAction(5)) {
        if (isFleeing()) {
            moveTo(getX(), getY() + 1);
            if (getY() >= VIEW_HEIGHT) {
                getWorld()->increaseScore(1000);
                getWorld()->playSound(SOUND_FINISHED_LEVEL);
                getWorld()->setLevelCompleted(true);
            }
        }
    }

    m_time_since_last_barrel++;
    // cerr << m_time_since_last_barrel << endl;

    
}

int Kong::euclidian_dist_w_player() {
    Actor * m_p = getWorld()->getPlayer();
    return sqrt(pow(m_p->getX() - getX(), 2) + pow(m_p->getY() - getY(), 2));
}


Koopa::Koopa(StudentWorld* sw, int x, int y) : Enemy(sw, IID_KOOPA, x, y, randInt(0, 1) == 0 ? left : right), m_freeze_cooldown(0), m_just_froze_something(false), m_delay(0) {
    // randomly set direction

}

bool Koopa::special_conditions(Actor * ptr) {
    return m_freeze_cooldown <= 0 && Attacker::special_conditions(ptr);
    // return m_freeze_cooldown <= 0;
}

void Koopa::specialization_attack(Actor * ptr) {
    m_freeze_cooldown = 50;
    m_just_froze_something = true;
}

void Koopa::special_death_actions() {
    if (randInt(0, 2) == 0) {
        ExtraLifeGoodie * m_extra = new ExtraLifeGoodie(getWorld(), getX(), getY());
        getWorld()->addObject(m_extra);
    }
}


void Koopa::doSomething() {
    if (isDead()) {
        return;
    }
    attack();

    if (m_freeze_cooldown > 0) {
        m_freeze_cooldown--;
    }

    // COMMENT BACK IN ONCE DONE DEBUGGING
    int dir = (getDirection() == left) ? -1 : 1;
    if (!m_just_froze_something && timeToDoAction(10)) {
        // m_delay = 0;
        bool alreadyMoved = false;
        if (!canMoveThere(getX() + dir, getY()) || canFallThere(getX() + dir, getY() - 1)) {
            if (!canMoveThere(getX() + dir, getY())) {
                // cerr << "reason: wall there" << endl;
            } 
            if (canFallThere(getX() + dir, getY() - 1)){
                // cerr << "reason: i'd fall!" << endl;
            } 
            // cerr << "can't go in that direction anymore, reversing" << endl;
            setDirection(dir == 1 ? left : right); // reverse direction of fireball
            dir = getDirection() == left ? -1 : 1;
            alreadyMoved = true;
            
        } 

    
        if (!alreadyMoved) {
            // else {
                // cerr << "going on my merry way!" << endl;
                moveTo(getX() + dir, getY());
        }

        attack();
    } 

    
    // m_delay++;
    m_just_froze_something = false;

}

Fireball::Fireball(StudentWorld* sw, int x, int y) : Enemy(sw, IID_FIREBALL, x, y, randInt(0, 1) == 0 ? left : right), m_climbing_state(0) {

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

void Fireball::doSomething() {
    if (isDead()) {
        return;
    }

    attack();

    if (timeToDoAction(10)) {
        // m_delay = 0;
        int dir = (getDirection() == left) ? -1 : 1;
        if (dir == 1) {
            // cerr << "going right!" << endl;
        } else {
            // cerr << "going left!" << endl;
        }
        // if (canMoveThere(getX(), getY()+1)) {
        //     cerr << "can move one up" << endl;
        // }
        // if (canClimbThere(getX(), getY()+1)) {
        //     cerr << "can climb one up" << endl;
        // }
        // if (canClimbThere(getX(), getY())) {
        //     cerr << "can climb here" << endl;
        // }
        bool alreadyMoved = false;
        if (canClimbThere(getX(), getY()) && (canMoveThere(getX(), getY()+1) || canClimbThere(getX(), getY() + 1)) && !isClimbingDown()) {
            // cerr << "deciding if i should climb up!" << endl;
            if (isClimbingUp() || randInt(0, 2) == 0) {
                setClimbingState(1);
                moveTo(getX(), getY() + 1); // move up, down, left, right can be turned into member functions of moving mortal
                alreadyMoved = true;
            }
            
        } else if (canClimbThere(getX(), getY() - 1) && !isClimbingUp()) {
            // cerr << "deciding if i should climb down!" << endl;
            if (isClimbingDown() || randInt(0, 2) == 0) {
                setClimbingState(-1);
                moveTo(getX(), getY() - 1);
                alreadyMoved = true;
            }
            
        } 

        if (!alreadyMoved) {
            if (isClimbing() && (!canClimbThere(getX(), getY() + getClimbingState()) || !canMoveThere(getX(), getY() + getClimbingState()))) {
                // cerr << "not climbing anymore" << endl;
                setClimbingState(0);
            }
            if (!canMoveThere(getX() + dir, getY()) || canFallThere(getX() + dir, getY() - 1)) {
                if (!canMoveThere(getX() + dir, getY())) {
                    // cerr << "reason: wall there" << endl;
                } 
                if (canFallThere(getX() + dir, getY() - 1)){
                    // cerr << "reason: i'd fall!" << endl;
                } 
                // cerr << "can't go in that direction anymore, reversing" << endl;
                setDirection(dir == 1 ? left : right); // reverse direction of fireball
                dir = getDirection() == left ? -1 : 1;
                alreadyMoved = true;
                
            } 

        }
        if (!alreadyMoved) {
            // else {
                // cerr << "going on my merry way!" << endl;
                moveTo(getX() + dir, getY());
            
        }

        attack();

        // cerr << "----" << endl;

    }

    
    // m_delay++;

}

// void Fireball::getAttacked(Actor * ptr) {
//     setDead(true);
//     getWorld()->playSound(SOUND_ENEMY_DIE);
// }

void Fireball::special_death_actions() {
    if (randInt(0, 2) == 0) {
        GarlicGoodie* m_garlic = new GarlicGoodie(getWorld(), getX(), getY());
        getWorld()->addObject(m_garlic);
    }
}

Barrel::Barrel(StudentWorld * sw, int x, int y, int dir) : Enemy(sw, IID_BARREL, x, y, dir) {

}

void Barrel::doSomething() {
    if (isDead()) {
        return;
    }

    if(canFallThere(getX(), getY() - 1) || canClimbThere(getX(), getY() - 1)) {
        moveTo(getX(), getY() - 1);

        if(!canMoveThere(getX(), getY() - 1)) {
            setDirection(getDirection() == left ? right : left);
        }
    }
    


    if(timeToDoAction(10)) {
        int dir = getDirection() == left ? -1 : 1;
        if (!canMoveThere(getX() + dir, getY())) {
            //cerr << "barrel can't move there" << endl;
            setDirection(getDirection() == left ? right : left);
        } else {
            moveTo(getX() + dir, getY());
        }

        
    }
    attack();

}

void Barrel::getAttacked(Actor * ptr) {
    if (!ptr->isDestructive()) {
        getWorld()->increaseScore(100);
        getWorld()->playSound(SOUND_ENEMY_DIE);
        //cerr << "something else is attacking me!" << endl;
    } 
    setDead(true);
    return;
    
}
