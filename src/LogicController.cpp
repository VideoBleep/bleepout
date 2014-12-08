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
  Player* previousPlayer = e.ball()->player();
  Player* player = e.object()->player();
  e.ball()->setPlayer(player);
  notifyBallOwnerChanged(e.ball(), player, previousPlayer);
}

void LogicController::onBallHitBrick(BallHitBrickEventArgs &e) {
  Ball* ball = e.ball();
  Brick* brick = e.object();
  Player* player = ball->player();
  
  brick->kill();
  notifyBrickDestroyed(brick, ball);
  
  if (player) {
    player->addScore(brick->value());
    notifyPlayerScoreChanged(player);
  }
  //...
}

void LogicController::onBallHitWall(BallHitWallEventArgs &e) {
  Wall* wall = e.object();
  if (wall->isExit()) {
    Ball* ball = e.ball();
    Player* player = ball->player();
  }
}

void LogicController::onBallHitBall(BallHitBallEventArgs &e) {
  //...
}
