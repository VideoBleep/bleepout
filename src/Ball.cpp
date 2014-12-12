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
}

Ball::~Ball() {
  ofLogVerbose() << "Destroy Ball";
}

void Ball::bounce(ofVec3f normal) {
    if (trajectory) {
        trajectory->reflect(normal);
    }
}
