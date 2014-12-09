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
  destroy();
}

void Ball::output(std::ostream &os) const {
  auto pos = body->GetPosition();
  os << "Ball{id:" << id() << ", pos:(" << pos.x << "," << pos.y << ")";
  os << ", player:";
  if (_player)
    os << _player->id();
  else
    os << "(none)";
  os << "}";
}
