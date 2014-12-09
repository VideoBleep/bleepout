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
  ~Player() override;

  void setPaddle(ofPtr<Paddle> paddle) { _paddle = paddle; }
  ofPtr<ofxLibwebsockets::Connection> connection() { return _conn; }
  ofPtr<Paddle> paddle() { return _paddle; }
  
private:
  void output(std::ostream& os) const override;
  
  ofPtr<ofxLibwebsockets::Connection> _conn;
  ofPtr<Paddle> _paddle;
};

#endif /* defined(__bleepout__Player__) */
