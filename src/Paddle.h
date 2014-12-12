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
#include "PhysicsObject.h"
#include <ofTypes.h>

class Paddle : public GameObject, public PhysicsObject {
public:
  Paddle(Player* player);
  ~Paddle() override;
  
  Player* player() { return _player; }
  const Player* player() const { return _player; }
  
  void output(std::ostream& os) const override;
  
  static const char* typeName() { return "paddle"; }
private:
  Player* _player;
};


#endif /* defined(__bleepout__Paddle__) */
