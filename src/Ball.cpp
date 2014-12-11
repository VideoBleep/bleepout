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
    trajectory = new OrbitalTrajectory(170.0, ofVec3f(0, 1, 0), ofVec3f(ofRandom(-1,1), ofRandom(-1,1), ofRandom(-1,1)), 0.04);
    thisGameObject = this;
}

Ball::~Ball() {
  ofLogVerbose() << "Destroy Ball";
  delete trajectory;
}

void Ball::bounce() {
    trajectory->reflect(ofVec3f(1, -1, 1));
}
