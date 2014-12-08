//
//  Paddle.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Paddle.h"

Paddle::Paddle(Player* player)
: GameObject(GAME_OBJECT_PADDLE), _player(player) {
  ofLogVerbose() << "Create Paddle";
}

Paddle::~Paddle() {
  ofLogVerbose() << "Destroy Paddle";
  destroy();
}

void Paddle::output(std::ostream &os) const {
  // this shouldn't be necessary. getPosition should really be marked as const
  auto pos = const_cast<Paddle*>(this)->getPosition();
  os << "Paddle{id:" << id() << ", pos:(" << pos.x << "," << pos.y << ")";
  if (_player)
    os << ", player:" << _player->id();
  else
    os << ", player: NONE";
  os << "}";
}
