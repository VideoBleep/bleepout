//
//  Paddle.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Paddle.h"

Paddle::Paddle(Player* player)
: GameObject(GAME_OBJECT_PADDLE), PhysicsObject(CollisionBox), _player(player) {
  ofLogVerbose() << "Create Paddle";
  thisGameObject = this;
}

Paddle::~Paddle() {
  ofLogVerbose() << "Destroy Paddle";
}

void Paddle::output(std::ostream &os) const {
  ofVec3f pos = getPosition();
  os << "Paddle{id:" << id() << ", pos:" << pos << "}";
  if (_player)
    os << ", player:" << _player->id();
  else
    os << ", player: NONE";
  os << "}";
}
