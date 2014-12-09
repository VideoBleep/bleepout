//
//  Ball.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Ball.h"


Ball::Ball() : GameObject(GAME_OBJECT_BALL) {
  ofLogVerbose() << "Create Ball";
}

Ball::~Ball() {
  ofLogVerbose() << "Destroy Ball";
}

void Ball::output(std::ostream &os) const {
  auto pos = getPosition();
  os << "Ball{id:" << id() << ", pos:" << pos << "}";
  os << ", curPlayer:";
  if (_lastPlayer)
    os << _lastPlayer->id();
  else
    os << "(none)";
  os << "}";
}

float Ball::getRadius() const {
    return getDimensions().x;
}

void Ball::setRadius(float newRadius) {
    setDimensions(ofVec3f(newRadius, newRadius, newRadius));
}