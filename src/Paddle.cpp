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
  _color = ofColor(128, 128, 128);
}

const ofColor& Paddle::getColor() const {
    if (_player) {
        return _player->getColor();
    } else {
        return _color;
    }
}