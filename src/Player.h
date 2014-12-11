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
#include <Connection.h>

class Paddle;

class Player : public GameObject {
public:
  Player();
  
  void setPaddle(Paddle* paddle) { _paddle = paddle; }
  Paddle* paddle() { return _paddle; }
  const Paddle* paddle() const { return _paddle; }
  
  int score() const { return _score; }
  int adjustScore(int add) {
    _score += add;
    return _score;
  }
  
  int lives() const { return _lives; }
  void setLives(int lives) {
    if (lives <= 0) {
      _lives = 0;
      kill();
    } else {
      _lives = lives;
    }
  }
  int adjustLives(int amount) {
    setLives(_lives + amount);
    return _lives;
  }
  
  void output(std::ostream& os) const override;
  
  static const char* typeName() { return "player"; }
  
  ofPtr<ofxLibwebsockets::Connection> connection() { return _conn; }

private:
  
  Paddle* _paddle;
  int _score;
  int _lives;
  ofPtr<ofxLibwebsockets::Connection> _conn;
};

#endif /* defined(__bleepout__Player__) */
