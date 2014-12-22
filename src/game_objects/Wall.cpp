//
//  Wall.cpp
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#include "Wall.h"
#include "BleepoutConfig.h"
#include "CircularTrajectory.h"

const char GameObjectTypeTraits<Wall>::typeName[] = "wall";

Wall::Wall(const RoundConfig* config /*= NULL*/, const WallSpec* spec /*= NULL*/)
: GameObject(GAME_OBJECT_WALL), _visible(true)
{
    thisGameObject = this;
  if (config && spec) {
    _isExit = spec->isExit;
    _visible = spec->visible;
    this->setPositionSpherical(config->domeRadius() + config->domeMargin(), spec->elevation, spec->heading);
    this->setSize(spec->size);
    if (spec->speed == 0) {
      this->setPositionSpherical(config->domeRadius() +
                                 config->domeMargin(),
                                 spec->elevation,
                                 spec->heading);
    } else {
      this->setTrajectory(new CircularTrajectory(config->domeRadius() +
                                                 config->domeMargin(),
                                                 spec->speed,
                                                 spec->elevation,
                                                 spec->heading,
                                                 spec->stopHeading));
    }
  }
}