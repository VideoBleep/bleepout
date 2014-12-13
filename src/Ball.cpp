//
//  Ball.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Ball.h"
#include "OrbitalTrajectory.h"


Ball::Ball()
: GameObject(GAME_OBJECT_BALL)
, _player(NULL)
, PhysicsObject(CollisionSphere)
{
    ofLogVerbose() << "Create Ball";
    thisGameObject = this;
    _color = ofColor(220, 220, 220);
}

Ball::~Ball() {
  ofLogVerbose() << "Destroy Ball";
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

