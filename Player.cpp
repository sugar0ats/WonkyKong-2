#include "Player.h"
#include "StudentWorld.h"
#include "GameConstants.h"

Player::Player(StudentWorld* sw, int x, int y) : Actor(sw, IID_PLAYER, x, y) {

}

void Player::doSomething() {
    // react to left right inputs
    int ch;
    if (getWorld()->getKey(ch))
    {
    // user hit a key this tick!
        switch (ch)
        {
            case KEY_PRESS_LEFT: {
                //... turn or move left ...
                // if current direction isn't left, set it to left and break!
                if (getDirection() != left) { 
                    setDirection(left);
                    break;
                }
                cout << "check if i can move to " << getX() - 1 << " " << getY() << endl;
                if (!canMoveThere(getX() - 1, getY())) {
                    break;
                }
                moveTo(getX() - 1, getY());
                cout << "left!" << endl;
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
                cout << "check if i can move to " << getX() + 1 << " " << getY() << endl;
                if (!canMoveThere(getX() + 1, getY())) {
                    break;
                }
                moveTo(getX() + 1, getY());
                cout << "right!" << endl;
                break;
            }
            
            case KEY_PRESS_SPACE: {
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

bool Player::canMoveThere(int x, int y) {
    Actor * ptr = getWorld()->getPtr(x, y);
    if (ptr == nullptr) {
        return true;
    }
    cout << "returning a pointer to something" << endl;
    return !(getWorld()->getPtr(x, y)->isObstruction());
}