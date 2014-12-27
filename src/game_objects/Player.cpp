//
//  Player.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Player.h"
#include "Paddle.h"
#include <ofMain.h>

const char GameObjectTypeTraits<Player>::typeName[] = "player";

Player::Player() : GameObject(GAME_OBJECT_PLAYER), _conn(NULL) {
  init();
}

Player::Player(ofxLibwebsockets::Connection* conn)
: GameObject(GAME_OBJECT_PLAYER), _conn(conn) {
  init();
}

void Player::init() {
  _paddle = NULL;
  _score = 0;
  _lives = 3;
  _color.setHsb(ofRandom(255), 255, 200);
}

void Player::enqueueBallModifier(const ModifierSpec &modifierSpec) {
  _ballModifierQueue.push_back(modifierSpec);
  _modifierCounters.add(modifierSpec.type);
}

bool Player::tryDequeueBallModifier(ModifierSpec *modifierSpec) {
  if (_ballModifierQueue.empty())
    return false;
  *modifierSpec = _ballModifierQueue.front();
  _ballModifierQueue.pop_front();
  return true;
}

void Player::incrementModifierCount(ModifierType type) {
  _modifierCounters.add(type, 1);
}

bool ExtraLifeModifier::applyToTarget(RoundState& state, GameObject &target) {
  if (target.type() == GAME_OBJECT_PADDLE) {
    Paddle& paddle = static_cast<Paddle&>(target);
    return applyToTarget(state, paddle.player());
  }
  if (target.type() != GAME_OBJECT_PLAYER)
    return false;
  Player& player = static_cast<Player&>(target);
  player.adjustLives(1);
  player.incrementModifierCount(modifierType());
  kill();
  return true;
}