#include "Floor.h"
#include "GameConstants.h"

Floor::Floor(StudentWorld* sw, int x, int y) : Actor(sw, IID_FLOOR, x, y) {

}

void Floor::doSomething() {

}

bool Floor::isObstruction() const {
    return true;
}