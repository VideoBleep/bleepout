//
//  Paddle.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Paddle.h"

const char GameObjectTypeTraits<Paddle>::typeName[] = "paddle";

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

void Paddle::addWidthModifier(float amount) {
  if (!_hasWidthModifier) {
    _origSize = getSize();
    _hasWidthModifier = true;
  }
  setSize(_origSize * amount);
}

void Paddle::removeWidthModifier() {
  if (_hasWidthModifier) {
    setSize(_origSize);
    _hasWidthModifier = false;
  }
}

PaddleWidthModifier::PaddleWidthModifier(const ModifierSpec* spec)
: Modifier(MODIFIER_PADDLE_WIDTH)
, _amount(1.2f) {
  if (spec) {
    spec->getProperty("amount", &_amount);
  }
}

bool PaddleWidthModifier::applyToTarget(GameObject &target) {
  if (target.type() != GAME_OBJECT_PADDLE)
    return false;
  Paddle& paddle = static_cast<Paddle&>(target);
  paddle.addWidthModifier(_amount);
}
