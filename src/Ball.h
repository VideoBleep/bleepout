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
#include "PhysicsObject.h"
#include "Common.h"
#include <ofTypes.h>

class Ball : public GameObject, public PhysicsObject {
public:
  Ball();
  ~Ball() override;
  
  Player* player() { return _player; }
  void setPlayer(Player* player) { _player = player; }

  void output(std::ostream& os) const override;
  void bounce();

  static const char* typeName() { return "ball"; }
private:
  Player* _player;
};

#endif /* defined(__bleepout__Ball__) */
