//
//  Paddle.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Paddle.h"
#include "GameState.h"

const char GameObjectTypeTraits<Paddle>::typeName[] = "paddle";

Paddle::Paddle(Player* player, ofVec3f size)
: GameObject(GAME_OBJECT_PADDLE), PhysicsObject(CollisionBox), _player(player) {
  thisGameObject = this;
  _color = ofColor(128, 128, 128);
  setSize(size);
  _origSize = size;
}

const ofColor& Paddle::getColor() const {
    if (_player) {
        return _player->getColor();
    } else {
        return _color;
    }
}

void Paddle::addWidthModifier(RoundState& state,
                              PaddleWidthModifier& modifier) {
  if (!_hasWidthModifier) {
    _origSize = getSize();
    _hasWidthModifier = true;
  }
  setSize(_origSize * modifier.amount());
  _widthModifierExpiration = state.time + modifier.duration();
}

void Paddle::removeWidthModifier() {
  if (_hasWidthModifier) {
    setSize(_origSize);
    _hasWidthModifier = false;
  }
}

void Paddle::updateModifiers(RoundState &state) {
  if (_hasWidthModifier) {
    if (state.time > _widthModifierExpiration) {
      removeWidthModifier();
    }
  }
}

PaddleWidthModifier::PaddleWidthModifier(const ModifierSpec& spec)
: Modifier(spec)
, _amount(1.2f) {
  _amount = _spec.amount;
}

bool PaddleWidthModifier::applyToTarget(RoundState& state, GameObject &target) {
  if (target.type() != GAME_OBJECT_PADDLE)
    return false;
  Paddle& paddle = static_cast<Paddle&>(target);
  paddle.addWidthModifier(state, *this);
  return true;
}
