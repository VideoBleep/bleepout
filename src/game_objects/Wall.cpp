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
  float d = appParams.domeMargin + appParams.domeRadius;
  if (spec.isFloor) {
    this->setSize(ofVec3f(d * 10, 10, d * 10));
    this->setPosition(ofVec3f(0, spec.elevation, 0));
  } else {
    this->setSize(spec.size);
    if (spec.speed == 0) {
      this->setPositionSpherical(d,
                                 spec.elevation,
                                 spec.heading);
    } else {
      this->setTrajectory(new CircularTrajectory(d,
                                                 spec.speed,
                                                 spec.elevation,
                                                 spec.heading,
                                                 spec.stopHeading));
    }
  }
}