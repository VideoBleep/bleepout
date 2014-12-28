//
//  Modifier.cpp
//  bleepout
//
//  Created by tekt on 12/15/14.
//
//

#include "Modifier.h"
#include "Player.h"
#include "Paddle.h"
#include "Brick.h"
#include "Ball.h"
#include "BleepoutConfig.h"
#include "OrbitalTrajectory.h"
#include "PhysicsUtil.h"
#include "BleepoutParameters.h"

const char GameObjectTypeTraits<Modifier>::typeName[] = "modifier";

Modifier* Modifier::createModifier(const ModifierSpec &spec) {
  if (spec.type == MODIFIER_NONE)
    return NULL;
  switch (spec.type) {
    case MODIFIER_EXTRA_LIFE:
      return new ExtraLifeModifier(spec);
    case MODIFIER_PADDLE_WIDTH:
      return new PaddleWidthModifier(spec);
    case MODIFIER_LASER_BALL:
      return new BallModifier(spec);
    default:
      return NULL;
  }
}

Modifier::Modifier(const ModifierSpec& spec)
: GameObject(GAME_OBJECT_MODIFIER)
, _spec(spec)
, _visible(false)
, _physical(false) {
  thisGameObject = this;
}

void Modifier::setup(const RoundConfig &config,
                     const Brick &spawner) {
  const auto& appParams = BleepoutParameters::get();
  this->setSize(ofVec3f(config.modifierRadius()));
  auto t = new OrbitalTrajectory();
  t->setRadius(appParams.domeRadius + appParams.domeMargin);
  t->setSpeed(0.02);
  float heading;
  float elevation1;
  cartesianToSpherical(spawner.getPosition(), &elevation1, &heading);
  t->initWithTwoPoints(elevation1, heading, 0.0f, heading);
  this->setTrajectory(t);
  this->setColor(ofColor(127, 255, 0));
  _physical = true;
  _visible = true;
}
