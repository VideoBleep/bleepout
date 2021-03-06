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
#include "BleepoutParameters.h"

const char GameObjectTypeTraits<Brick>::typeName[] = "brick";

static std::string generateModifierName(const ModifierSourceSpec& mod) {
  if (!mod.name.empty()) {
    if (mod.chance >= 1 || ofRandomuf() < mod.chance) {
      return mod.name;
    }
  }
  return "";
}

Brick::Brick(const RoundConfig& config, const BrickSpec& spec)
: GameObject(GAME_OBJECT_BRICK)
, PhysicsObject(CollisionBox)
, _value(1), _lives(1), _maxLives(1)
{
  const auto& appParams = BleepoutParameters::get();
  thisGameObject = this;
  this->setSize(spec.size);
  this->setColor(spec.color);
  this->setLives(spec.lives);
  this->setValue(spec.value);
  _maxLives = spec.lives;
  _modifierName = generateModifierName(spec.modifier);
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
