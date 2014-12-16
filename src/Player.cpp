//
//  Player.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Player.h"
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