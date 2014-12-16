//
//  Wall.cpp
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#include "Wall.h"
#include "BleepoutConfig.h"

const char GameObjectTypeTraits<Wall>::typeName[] = "wall";

Wall::Wall(const RoundConfig* config /*= NULL*/, const WallSpec* spec /*= NULL*/)
: GameObject(GAME_OBJECT_WALL)
{
    thisGameObject = this;
  if (config && spec) {
    _isExit = spec->isExit;
    this->setPositionSpherical(config->domeRadius() + config->domeMargin(), spec->elevation, spec->heading);
    this->setSize(spec->size);
  }
}