//
//  Player.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Player.h"
#include <ofMain.h>

Player::Player() : GameObject(GAME_OBJECT_PLAYER), _paddle(NULL), _score(0), _lives(3) {
    _color.setHsb(ofRandom(255), 255, 200);
}

