//
//  Ball.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__Ball__
#define __bleepout__Ball__

#include "Player.h"
#include <ofMain.h>
#include <ofxBox2d.h>

class Ball {
public:
  ofxBox2dCircle& circle() { return _circle; }
  ofPtr<Player>& lastPlayer() { return _lastPlayer; }
  void setLastPlayer(ofPtr<Player> player) {
    _lastPlayer = player;
  }
private:
  ofxBox2dCircle _circle;
  ofPtr<Player> _lastPlayer;
};

#endif /* defined(__bleepout__Ball__) */
