//
//  Ball.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Ball.h"


Ball::Ball() : GameObject(GAME_OBJECT_BALL), _player(NULL) {
  ofLogVerbose() << "Create Ball";
}

Ball::~Ball() {
  ofLogVerbose() << "Destroy Ball";
  destroy();
}
