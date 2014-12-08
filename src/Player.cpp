//
//  Player.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Player.h"
#include <ofMain.h>

Player::Player() : GameObject(GAME_OBJECT_PLAYER) {
}

void Player::output(std::ostream &os) const {
  os << "Player{id:" << id() << "}";
}
