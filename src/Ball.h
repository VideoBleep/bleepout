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
#include <ofTypes.h>
#include <ofxBox2d.h>

class Ball : public GameObject, public ofxBox2dCircle {
public:
  Ball();
  ~Ball() override;
  
  Player* player() { return _player; }
  void setPlayer(Player* player) { _player = player; }

  void output(std::ostream& os) const override;
  
  static const char* typeName() { return "ball"; }
private:
  Player* _player;
};

#endif /* defined(__bleepout__Ball__) */
