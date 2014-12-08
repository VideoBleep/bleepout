//
//  Player.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__Player__
#define __bleepout__Player__

#include "GameObject.h"
#include <iostream>
#include <ofTypes.h>

class Paddle;

class Player : public GameObject {
public:
  Player();
  
  void setPaddle(Paddle* paddle) { _paddle = paddle; }
  Paddle* paddle() { return _paddle; }
  
  int score() const { return _score; }
  
  int addScore(int add) {
    _score += add;
    return _score;
  }

private:
  void output(std::ostream& os) const override;
  
  Paddle* _paddle;
  int _score;
};

#endif /* defined(__bleepout__Player__) */
