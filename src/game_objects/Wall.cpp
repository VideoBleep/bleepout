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
#include "BleepoutParameters.h"

const char GameObjectTypeTraits<Wall>::typeName[] = "wall";

Wall::Wall(const RoundConfig& config, const WallSpec& spec)
: GameObject(GAME_OBJECT_WALL), _visible(true)
{
  const auto& appParams = BleepoutParameters::get();
  thisGameObject = this;
  _isExit = spec.isExit;
  _visible = spec.visible;
  this->setPositionSpherical(appParams.domeRadius + appParams.domeMargin, spec.elevation, spec.heading);
  this->setSize(spec.size);
  if (spec.speed == 0) {
    this->setPositionSpherical(appParams.domeRadius +
                               appParams.domeMargin,
                               spec.elevation,
                               spec.heading);
  } else {
    this->setTrajectory(new CircularTrajectory(appParams.domeRadius +
                                               appParams.domeMargin,
                                               spec.speed,
                                               spec.elevation,
                                               spec.heading,
                                               spec.stopHeading));
  }
}