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

const char GameObjectTypeTraits<Ball>::typeName[] = "ball";

Ball::Ball(const RoundConfig* config /*= NULL*/, const BallSpec* spec /*= NULL*/)
: GameObject(GAME_OBJECT_BALL)
, _player(NULL)
, PhysicsObject(CollisionSphere)
{
    thisGameObject = this;
    _color = ofColor(220, 220, 220);
    if (config && spec) {
        this->setSize(ofVec3f(config->ballRadius()));
        auto t = new OrbitalTrajectory();
        t->setRadius(config->domeRadius() + config->domeMargin());
        t->setSpeed(0.03);
        t->initWithTwoPoints(spec->elevation, spec->heading, -14,
                             spec->heading + ofRandom(-45, 45));
        this->setTrajectory(t);
    }
}

void Ball::bounce(ofVec3f normal) {
    if (trajectory) {
        trajectory->reflect(normal);
    }
}

const ofColor& Ball::getColor() const {
    if (_player) {
        return _player->getColor();
    } else {
        return _color;
    }
}

bool Ball::isLaser() const {
  return _laserModifier.active();
}

void Ball::applyModifier(const RoundState &state,
                         const ModifierSpec &modifierSpec) {
  _laserModifier.set(modifierSpec, state.time);
}

const ModifierSpec* Ball::removeLaserModifier() {
  if (_laserModifier.active()) {
    return _laserModifier.clear();
  }
  return NULL;
}

const ModifierSpec* Ball::updateLaserModifier(const RoundState &state) {
  if (_laserModifier.active() &&
      _laserModifier.checkExpiration(state.time)) {
    return removeLaserModifier();
  }
  return NULL;
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
