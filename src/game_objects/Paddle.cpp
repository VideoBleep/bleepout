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

Paddle::Paddle(Player& player, ofVec3f size)
: GameObject(GAME_OBJECT_PADDLE), PhysicsObject(CollisionBox), _player(player) {
  thisGameObject = this;
  _color = ofColor(128, 128, 128);
  setSize(size);
  _origSize = size;
}

const ofColor& Paddle::getColor() const {
  return _player.getColor();
}

void Paddle::addWidthModifier(const RoundState& state,
                              const PaddleWidthModifier& modifier) {
  _widthModifier.set(modifier.spec(), state.time);
  setSize(_origSize * modifier.amount());
  _player.incrementModifierCount(modifier.modifierType());
}

const ofPtr<ModifierSpec> Paddle::removeWidthModifier() {
  if (_widthModifier.active()) {
    setSize(_origSize);
    return _widthModifier.clear();
  }
  return ofPtr<ModifierSpec>();
}

const ofPtr<ModifierSpec> Paddle::updateWidthModifier(const RoundState &state) {
  if (_widthModifier.active() &&
      _widthModifier.checkExpiration(state.time)) {
    return removeWidthModifier();
  }
  return ofPtr<ModifierSpec>();
}

PaddleWidthModifier::PaddleWidthModifier(const ModifierSpec& spec)
: Modifier(spec) { }

bool PaddleWidthModifier::applyToTarget(RoundState& state, GameObject &target) {
  if (target.type() != GAME_OBJECT_PADDLE)
    return false;
  Paddle& paddle = static_cast<Paddle&>(target);
  paddle.addWidthModifier(state, *this);
  return true;
}
