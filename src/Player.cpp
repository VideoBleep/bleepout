//
//  Player.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Player.h"

void Player::output(std::ostream &os) const {
  os << "Player{id:" << id() << "}";
}
