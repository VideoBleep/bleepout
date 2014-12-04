//
//  Paddle.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__Paddle__
#define __bleepout__Paddle__

#include "Player.h"
#include "GameObject.h"
#include <ofxBox2d.h>

class Paddle : public GameObject {
public:
  Paddle(Player& player) : GameObject(GAME_OBJECT_PADDLE), _player(player) {}

  ofxBox2dRect& rect() { return _rect; };
  
  Player& player() { return _player; };

private:
  Player& _player;
  ofxBox2dRect _rect;
};


#endif /* defined(__bleepout__Paddle__) */
