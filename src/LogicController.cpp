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
  if (player != previousPlayer) {
    notifyBallOwnerChanged(_state, e.ball(), player, previousPlayer);
  }
}

void LogicController::onBallHitBrick(BallHitBrickEventArgs &e) {
  Ball* ball = e.ball();
  Brick* brick = e.object();
  Player* player = ball->player();
  
  if (player) {
    brick->kill();
    _state.decrementLiveBricks();
    notifyBrickDestroyed(_state, brick, ball);
    
    player->adjustScore(brick->value());
    notifyPlayerScoreChanged(_state, player);
    
    if (_state.liveBricks() <= 0) {
      notifyAllBricksDestroyed(_state);
    }
  }
  //...
}

void LogicController::onBallHitWall(BallHitWallEventArgs &e) {
  Wall* wall = e.object();
  if (wall->isExit()) {
    Ball* ball = e.ball();
    Player* player = ball->player();
    
    notifyBallDestroyed(_state, ball);
    
    if (player) {
      player->adjustLives(-1);
      notifyPlayerLivesChanged(_state, player);
      if (!player->alive()) {
        notifyPlayerLost(_state, player);
      }
    }
  }
}

void LogicController::onBallHitBall(BallHitBallEventArgs &e) {
  //...
}
