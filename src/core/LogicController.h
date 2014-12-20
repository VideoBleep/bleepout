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
  void onBallHitPaddle(BallHitPaddleEventArgs& e);
  void onBallHitBrick(BallHitBrickEventArgs& e);
  void onBallHitWall(BallHitWallEventArgs& e);
  void onBallHitBall(BallHitBallEventArgs& e);
  void onModifierHitPaddle(ModifierHitPaddleEventArgs& e);

  RoundState& _state;
  RoundConfig& _config;
};

#endif /* defined(__bleepout__LogicController__) */
