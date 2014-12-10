//
//  Ball.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Ball.h"


Ball::Ball()
: GameObject(GAME_OBJECT_BALL)
, _player(NULL)
, trajectory(1000.0, ofVec3f(0, 1, 0), ofVec3f(-1, 0, 0), 0.05)
{
  ofLogVerbose() << "Create Ball";
    
}

Ball::~Ball() {
  ofLogVerbose() << "Destroy Ball";
}

void Ball::output(std::ostream &os) const {
  auto pos = getPosition();
  os << "Ball{id:" << id() << ", pos:" << pos << "}";
  os << ", player:";
  if (_player)
    os << _player->id();
  else
    os << "(none)";
  os << "}";
}

void Ball::updatePositionFromTrajectory() {
    //trajectory.tick();
    //setPosition(trajectory.getPosition());
    
    ofVec3f pos = getPosition();
    pos.y += trajectory.getSpeed() * 10;
    setPosition(pos);
}

void Ball::bounce() {
    trajectory.setSpeed(-1 * trajectory.getSpeed());
}