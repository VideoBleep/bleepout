//
//  LogicController.h
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#ifndef __bleepout__LogicController__
#define __bleepout__LogicController__

#include "GameState.h"
#include "GameEvents.h"
#include "BleepoutConfig.h"

class LogicController : public RoundStateEventSource {
public:
  LogicController(RoundState& state, RoundConfig& config);
  
  void setup();
  void update();
  
  void attachTo(CollisionEventSource& collisions);
  void detachFrom(CollisionEventSource& collisions);
  
private:
  void onCollision(CollisionEventArgs& e);
  
  void onBallHitObject(Ball& ball, GameObject& object);
  void onModifierHitObject(Modifier& modifier, GameObject& object);
  
  void onBallHitPaddle(Ball& ball, Paddle& paddle);
  void onBallHitBrick(Ball& ball, Brick& brick);
  void onBallHitWall(Ball& ball, Wall& wall);
  void onBallHitBall(Ball& ball, Ball& otherBall);
  void onModifierHitPaddle(Modifier& modifier, Paddle& paddle);

  RoundState& _state;
  RoundConfig& _config;
};

#endif /* defined(__bleepout__LogicController__) */
