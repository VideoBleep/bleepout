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
#include "GameObject.h"
#include "Common.h"
#include <ofMain.h>
#include <ofxBox2d.h>

class Ball : public GameObject {
public:
  Ball();
  ~Ball() override;
  
  ofxBox2dCircle& circle() { return _circle; }
  const ofxBox2dCircle& circle() const { return _circle; }
  
  Player* lastPlayer() { return _lastPlayer; }
  void setLastPlayer(Player* player) {
    _lastPlayer = player;
  }
  void output(std::ostream& os) const override;
private:
  ofxBox2dCircle _circle;
  Player* _lastPlayer;
};

#endif /* defined(__bleepout__Ball__) */
