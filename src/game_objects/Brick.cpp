//
//  Brick.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Brick.h"
#include "BleepoutConfig.h"
#include "CircularTrajectory.h"

const char GameObjectTypeTraits<Brick>::typeName[] = "brick";

Brick::Brick(const RoundConfig* config /*= NULL*/, const BrickSpec* spec /*= NULL*/)
: GameObject(GAME_OBJECT_BRICK)
, PhysicsObject(CollisionBox)
, _value(1), _lives(1), _maxLives(1)
{
  thisGameObject = this;
  if (config && spec) {
    this->setSize(config->brickSize());
    this->setColor(spec->color);
    this->setLives(spec->lives);
    this->setValue(spec->value);
    _maxLives = spec->lives;
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
