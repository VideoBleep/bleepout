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
  Paddle(Player& player, ofVec3f size);
  
  Player& player() { return _player; }
  const Player& player() const { return _player; }
  
  void output(std::ostream& os) const override;
  
  virtual const ofColor& getColor() const override;
  
  const ModifierSpec* updateWidthModifier(const RoundState& state);
  
  void addWidthModifier(const RoundState& state,
                        const PaddleWidthModifier& modifier);
  const ModifierSpec* removeWidthModifier();
  
private:
  Player& _player;
  ofVec3f _origSize;
  ModifierSlot _widthModifier;
};


template<>
struct GameObjectTypeTraits<Paddle> {
  static const GameObjectType typeId = GAME_OBJECT_PADDLE;
  static const char typeName[];
};

class PaddleWidthModifier : public Modifier {
public:
  PaddleWidthModifier(const ModifierSpec& spec);
  virtual bool applyToTarget(RoundState& state, GameObject& target) override;
  virtual void output(std::ostream& os) const override;
  float amount() const { return spec().amount; }
};

#endif /* defined(__bleepout__Paddle__) */
