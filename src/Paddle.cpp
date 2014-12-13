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
  thisGameObject = this;
}
