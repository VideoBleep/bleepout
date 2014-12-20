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
#include <iostream>
#include "Modifier.h"

class Paddle : public GameObject, public PhysicsObject {
public:
  Paddle(Player* player);
  
  Player* player() { return _player; }
  const Player* player() const { return _player; }
  
  void output(std::ostream& os) const override;
    
  virtual const ofColor& getColor() const override;
  
  void addWidthModifier(float amount);
  void removeWidthModifier();
  
private:
  Player* _player;
  ofVec3f _origSize;
  bool _hasWidthModifier;
};


template<>
struct GameObjectTypeTraits<Paddle> {
  static const GameObjectType typeId = GAME_OBJECT_PADDLE;
  static const char typeName[];
};

class PaddleWidthModifier : public Modifier {
public:
  PaddleWidthModifier(const ModifierSpec* spec);
  virtual bool applyToTarget(GameObject& target) override;
  virtual void output(std::ostream& os) const override;
private:
  float _amount;
};

#endif /* defined(__bleepout__Paddle__) */
