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

void LogicController::attachTo(CollisionEventSource &collisions) {
  ofAddListener(collisions.ballHitPaddleEvent, this,
                &LogicController::onBallHitPaddle);
  ofAddListener(collisions.ballHitBrickEvent, this,
                &LogicController::onBallHitBrick);
  ofAddListener(collisions.ballHitWallEvent, this,
                &LogicController::onBallHitWall);
  ofAddListener(collisions.ballHitBallEvent, this,
                &LogicController::onBallHitBall);
  ofAddListener(collisions.modifierHitPaddleEvent, this,
                &LogicController::onModifierHitPaddle);
}

void LogicController::detachFrom(CollisionEventSource &collisions) {
  ofRemoveListener(collisions.ballHitPaddleEvent, this,
                   &LogicController::onBallHitPaddle);
  ofRemoveListener(collisions.ballHitBrickEvent, this,
                   &LogicController::onBallHitBrick);
  ofRemoveListener(collisions.ballHitWallEvent, this,
                   &LogicController::onBallHitWall);
  ofRemoveListener(collisions.ballHitBallEvent, this,
                   &LogicController::onBallHitBall);
  ofRemoveListener(collisions.modifierHitPaddleEvent, this,
                   &LogicController::onModifierHitPaddle);
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
    brick->adjustLives(-1);
    if (!brick->alive()) {
      brick->kill();
      _state.decrementLiveBricks();
      notifyBrickDestroyed(_state, brick, ball);
      
      const std::string& modifierName = brick->modifierName();
      if (!modifierName.empty()) {
        const ModifierSpec& spec = _config.modifierDef(modifierName);
        ofPtr<Modifier> modifier(Modifier::createModifier(spec));
        if (modifier) {
          modifier->setup(_config, *brick);
          _state.addModifier(modifier);
          notifyModifierAppeared(_state, modifier.get(), brick);
        }
      }
      
      player->adjustScore(brick->value());
      notifyPlayerScoreChanged(_state, player);
      
      if (_state.liveBricks() <= 0) {
        notifyAllBricksDestroyed(_state);
      }
    }
  }
}

void LogicController::onBallHitWall(BallHitWallEventArgs &e) {
  Wall* wall = e.object();
  if (wall->isExit()) {
    Ball* ball = e.ball();
    Player* player = ball->player();
    
    ball->kill();
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

void LogicController::onModifierHitPaddle(ModifierHitPaddleEventArgs &e) {
  Modifier& modifier = *e.modifier();
  if (modifier.applyToTarget(*e.paddle())) {
    notifyModifierRemoved(_state, &modifier, e.paddle());
    _state.modifiers().eraseObjectById(modifier.id());
    //...?
  }
  //...
}
