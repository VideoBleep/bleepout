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
#include "Player.h"
#include "Brick.h"

class LogicController : public RoundStateEventSource {
public:
  LogicController(RoundState& state, RoundConfig& config);
  
  void setup();
  void update();
  
  void initPlayer(Player& player);
  
  void onBallHitPaddle(BallHitPaddleEventArgs& e);
  void onBallHitBrick(BallHitBrickEventArgs& e);
  void onBallHitWall(BallHitWallEventArgs& e);
  void onBallHitBall(BallHitBallEventArgs& e);
  
private:
  RoundState& _state;
  RoundConfig& _config;
};

#endif /* defined(__bleepout__LogicController__) */
