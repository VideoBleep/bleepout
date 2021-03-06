//
//  Ball.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Ball.h"
#include "Paddle.h"
#include "OrbitalTrajectory.h"
#include "BleepoutConfig.h"
#include "GameState.h"
#include "BleepoutParameters.h"

const char GameObjectTypeTraits<Ball>::typeName[] = "ball";

Ball::Ball(const RoundConfig& config, const BallSpec& spec)
: GameObject(GAME_OBJECT_BALL)
, _player(NULL)
, PhysicsObject(CollisionSphere)
{
  const auto& appParams = BleepoutParameters::get();
  thisGameObject = this;
  _color = ofColor(90, 90, 70);
  this->setSize(ofVec3f(config.ballRadius()));
  auto t = new OrbitalTrajectory();
  t->setRadius(appParams.domeRadius + appParams.domeMargin);
  t->setSpeed(config.ballSpeed);
  _baseSpeed = config.ballSpeed;
  t->initWithTwoPoints(spec.elevation, spec.heading, -14,
                       spec.heading + ofRandom(-45, 45));
  this->setTrajectory(t);
}

void Ball::bounce(ofVec3f normal, float trueHitFactor /* = 0.0 */) {
  if (trajectory) {
    trajectory->reflect(normal, trueHitFactor);
  }
}

void Ball::updateSpeed() {
  if (trajectory) {
    trajectory->setSpeed(_baseSpeed * BleepoutParameters::get().ballSpeedMultiplier);
  }
}

const ofColor& Ball::getColor() const {
  if (_player) {
    return _player->getColor();
  } else {
    return _color;
  }
}

bool Ball::isSupercharged() const {
  return _modifier.active() && _modifier.spec()->type == MODIFIER_SUPERCHARGED_BALL;
}

void Ball::applyModifier(const RoundState &state,
                         const ModifierSpec &modifierSpec) {
  _modifier.set(modifierSpec, state.time);
}

const ofPtr<ModifierSpec> Ball::removeModifier() {
  if (_modifier.active()) {
    return _modifier.clear();
  }
  return ofPtr<ModifierSpec>();
}

const ofPtr<ModifierSpec> Ball::updateModifier(const RoundState &state) {
  if (_modifier.active() &&
      _modifier.checkExpiration(state.time)) {
    return removeModifier();
  }
  return ofPtr<ModifierSpec>();
}

bool BallModifier::applyToTarget(RoundState &state, GameObject &target) {
  if (target.type() == GAME_OBJECT_PADDLE) {
    Paddle& paddle = static_cast<Paddle&>(target);
    paddle.player().enqueueBallModifier(spec());
    return true;
  } else if (target.type() == GAME_OBJECT_PLAYER) {
    // i don't think this will ever happen, but might as well have it in here anyway
    Player& player = static_cast<Player&>(target);
    player.enqueueBallModifier(spec());
    return true;
  }
  return false;
}
