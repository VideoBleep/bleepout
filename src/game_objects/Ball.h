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

class RoundConfig;
struct BallSpec;

class Ball : public GameObject, public PhysicsObject {
public:
  Ball(const RoundConfig* config = NULL, const BallSpec* spec = NULL);
  
  Player* player() { return _player; }
  const Player* player() const { return _player; }
  void setPlayer(Player* player) { _player = player; }

  virtual const ofColor& getColor() const override;
    
  void output(std::ostream& os) const override;
  void bounce(ofVec3f normal);

private:
  Player* _player;
};

template<>
struct GameObjectTypeTraits<Ball> {
  static const GameObjectType typeId = GAME_OBJECT_BALL;
  static const char typeName[];
};

#endif /* defined(__bleepout__Ball__) */
