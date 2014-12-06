//
//  Paddle.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Paddle.h"

Paddle::Paddle(Player& player)
: GameObject(GAME_OBJECT_PADDLE), _player(player) {
  ofLogVerbose() << "Create Paddle";
}

Paddle::~Paddle() {
  ofLogVerbose() << "Destroy Paddle";
  _rect.destroy();
}

void Paddle::output(std::ostream &os) const {
  auto pos = _rect.body->GetPosition();
  os << "Paddle{id:" << id() << ", pos:(" << pos.x << "," << pos.y << ")";
  os << ", player:" << _player.id() << "}";
}
