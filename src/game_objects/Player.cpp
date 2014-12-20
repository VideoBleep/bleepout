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

bool ExtraLifeModifier::applyToTarget(GameObject &target) {
  if (target.type() == GAME_OBJECT_PADDLE) {
    Paddle& paddle = static_cast<Paddle&>(target);
    if (!paddle.player())
      return false;
    return applyToTarget(*paddle.player());
  }
  if (target.type() != GAME_OBJECT_PLAYER)
    return false;
  Player& player = static_cast<Player&>(target);
  player.adjustLives(1);
  kill();
  return true;
}