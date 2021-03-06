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
#include "Modifier.h"

class RoundConfig;
class RoundState;
struct BallSpec;

class Ball : public GameObject, public PhysicsObject {
public:
  Ball(const RoundConfig& config, const BallSpec& spec);
  
  Player* player() { return _player; }
  const Player* player() const { return _player; }
  void setPlayer(Player* player) { _player = player; }
  
  bool isSupercharged() const;
  
  virtual const ofColor& getColor() const override;
  
  void output(std::ostream& os) const override;
  void bounce(ofVec3f normal, float trueHitFactor = 0.0);
  
  void updateSpeed();
  
  void applyModifier(const RoundState& state,
                     const ModifierSpec& modifierSpec);
  
  const ofPtr<ModifierSpec> updateModifier(const RoundState& state);
  const ofPtr<ModifierSpec> removeModifier();
  
private:
  Player* _player;
  ModifierSlot _modifier;
  float _baseSpeed;
};

template<>
struct GameObjectTypeTraits<Ball> {
  static const GameObjectType typeId = GAME_OBJECT_BALL;
  static const char typeName[];
};

class BallModifier : public Modifier {
public:
  BallModifier(const ModifierSpec &spec)
  : Modifier(spec) { }
  virtual bool applyToTarget(RoundState& state, GameObject& target) override;
  virtual void output(std::ostream& os) const override;
};

#endif /* defined(__bleepout__Ball__) */
