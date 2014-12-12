//
//  Wall.cpp
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#include "Wall.h"

Wall::Wall(bool isExit)
: GameObject(GAME_OBJECT_WALL)
, _isExit(isExit)
{
    thisGameObject = this;
}