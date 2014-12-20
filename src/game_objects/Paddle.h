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

class RoundState;
class PaddleWidthModifier;

class Paddle : public GameObject, public PhysicsObject {
public:
  Paddle(Player* player, ofVec3f size);
  
  Player* player() { return _player; }
  const Player* player() const { return _player; }
  
  void output(std::ostream& os) const override;
    
  virtual const ofColor& getColor() const override;
  
  void updateModifiers(RoundState& state);
  
  void addWidthModifier(RoundState& state,
                        PaddleWidthModifier& modifier);
  void removeWidthModifier();
  
private:
  Player* _player;
  ofVec3f _origSize;
  bool _hasWidthModifier;
  float _widthModifierExpiration;
};


template<>
struct GameObjectTypeTraits<Paddle> {
  static const GameObjectType typeId = GAME_OBJECT_PADDLE;
  static const char typeName[];
};

class PaddleWidthModifier : public Modifier {
public:
  PaddleWidthModifier(const ModifierSpec* spec);
  virtual bool applyToTarget(RoundState& state, GameObject& target) override;
  virtual void output(std::ostream& os) const override;
  float amount() const { return _amount; }
  virtual float duration() const override { return 4.0f; }
private:
  float _amount;
};

#endif /* defined(__bleepout__Paddle__) */
