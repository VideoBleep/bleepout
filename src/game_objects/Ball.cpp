//
//  Ball.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Ball.h"
#include "OrbitalTrajectory.h"
#include "BleepoutConfig.h"

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

