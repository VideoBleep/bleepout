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
  _circle.destroy();
}

void Ball::output(std::ostream &os) const {
  auto pos = _circle.body->GetPosition();
  os << "Ball{id:" << id() << ", pos:(" << pos.x << "," << pos.y << ")";
  os << ", curPlayer:";
  if (_lastPlayer)
    os << _lastPlayer->id();
  else
    os << "(none)";
  os << "}";
}
