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
  
  Player* player() { return _player; }
  const Player* player() const { return _player; }
  
  void output(std::ostream& os) const override;
    
  virtual const ofColor& getColor() const override;
  
private:
  Player* _player;
};


template<>
struct GameObjectTypeTraits<Paddle> {
  static const GameObjectType typeId = GAME_OBJECT_PADDLE;
  static const char typeName[];
};

#endif /* defined(__bleepout__Paddle__) */
