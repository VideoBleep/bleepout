//
//  LogicController.cpp
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#include "LogicController.h"

LogicController::LogicController(RoundState& state, RoundConfig& config)
:_state(state), _config(config) {}

void LogicController::setup() {
  
}

void LogicController::update() {
  
}

void LogicController::onBallHitPaddle(BallHitPaddleEventArgs &e) {
  Player* previousPlayer = e.ball()->lastPlayer().get();
  ofPtr<Player> player = e.object()->player();
  e.ball()->setLastPlayer(player);
  notifyBallOwnerChanged(e.ball(), player.get(), previousPlayer);
}

void LogicController::onBallHitBrick(BallHitBrickEventArgs &e) {
  //...
}

void LogicController::onBallHitWall(BallHitWallEventArgs &e) {
  Ball* ball = e.ball();
  Wall* wall = e.object();
  if (wall->isExit()) {
  }
}

void LogicController::onBallHitBall(BallHitBallEventArgs &e) {
  //...
}

void LogicController::playerBallOut(Player &player, Ball &ball) {
  // ball destroyed!!!
  //???
}
