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

