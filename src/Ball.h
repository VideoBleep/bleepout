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

class Ball : public GameObject, public ofxBox2dCircle {
public:
  Ball();
  ~Ball() override;
  
  Player* lastPlayer() { return _lastPlayer; }
  void setLastPlayer(Player* player) {
    _lastPlayer = player;
  }
  void output(std::ostream& os) const override;
private:
  Player* _lastPlayer;
};

#endif /* defined(__bleepout__Ball__) */
