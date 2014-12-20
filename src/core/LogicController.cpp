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
  ofAddListener(collisions.collisionEvent, this,
                &LogicController::onCollision);
}

void LogicController::detachFrom(CollisionEventSource &collisions) {
  ofRemoveListener(collisions.collisionEvent, this,
                   &LogicController::onCollision);
}

void LogicController::update() {
  for (auto& obj : _state.paddles()) {
    if (obj && obj->alive()) {
      obj->updateModifiers(_state);
    }
  }
}

void LogicController::onCollision(CollisionEventArgs &e) {
  GameObject& objA = *e.a();
  GameObject& objB = *e.b();
  auto typeA = objA.type();
  auto typeB = objB.type();
  if (typeA == GAME_OBJECT_MODIFIER) {
    onModifierHitObject(static_cast<Modifier&>(objA), objB);
  } else if (typeB == GAME_OBJECT_MODIFIER) {
    onModifierHitObject(static_cast<Modifier&>(objB), objA);
  } else if (typeA == GAME_OBJECT_BALL) {
    onBallHitObject(static_cast<Ball&>(objA), objB);
  } else if (typeB == GAME_OBJECT_BALL) {
    onBallHitObject(static_cast<Ball&>(objB), objA);
  }
}

void LogicController::onBallHitObject(Ball &ball, GameObject &object) {
  switch (object.type()) {
    case GAME_OBJECT_BRICK:
      onBallHitBrick(ball, static_cast<Brick&>(object));
      break;
    case GAME_OBJECT_PADDLE:
      onBallHitPaddle(ball, static_cast<Paddle&>(object));
      break;
    case GAME_OBJECT_BALL:
      onBallHitBall(ball, static_cast<Ball&>(object));
      break;
    case GAME_OBJECT_WALL:
      onBallHitWall(ball, static_cast<Wall&>(object));
      break;
    default:
      break;
  }
}

void LogicController::onModifierHitObject(Modifier &modifier, GameObject &object) {
  switch (object.type()) {
    case GAME_OBJECT_PADDLE:
      onModifierHitPaddle(modifier, static_cast<Paddle&>(object));
      break;
    default:
      break;
  }
}

void LogicController::onBallHitPaddle(Ball& ball, Paddle& paddle) {
  Player* previousPlayer = ball.player();
  Player* player = paddle.player();
  ball.setPlayer(player);
  if (player != previousPlayer) {
    notifyBallOwnerChanged(_state, &ball, player, previousPlayer);
  }
}

void LogicController::onBallHitBrick(Ball& ball, Brick& brick) {
  Player* player = ball.player();
  if (player) {
    brick.adjustLives(-1);
    if (!brick.alive()) {
      brick.kill();
      _state.decrementLiveBricks();
      notifyBrickDestroyed(_state, &brick, &ball);
      
      const std::string& modifierName = brick.modifierName();
      if (!modifierName.empty()) {
        const ModifierSpec& spec = _config.modifierDef(modifierName);
        ofPtr<Modifier> modifier(Modifier::createModifier(spec));
        if (modifier) {
          modifier->setup(_config, brick);
          _state.addModifier(modifier);
          notifyModifierAppeared(_state, modifier.get(), &brick);
        }
      }
      
      player->adjustScore(brick.value());
      notifyPlayerScoreChanged(_state, player);
      
      if (_state.liveBricks() <= 0) {
        notifyAllBricksDestroyed(_state);
      }
    }
  }
}

void LogicController::onBallHitWall(Ball& ball, Wall& wall) {
  if (wall.isExit()) {
    Player* player = ball.player();
    
    ball.kill();
    notifyBallDestroyed(_state, &ball);
    
    if (player) {
      player->adjustLives(-1);
      notifyPlayerLivesChanged(_state, player);
      if (!player->alive()) {
        notifyPlayerLost(_state, player);
      }
    }
  }
}

void LogicController::onBallHitBall(Ball& ball, Ball& otherBall) {
  //...
}

void LogicController::onModifierHitPaddle(Modifier& modifier, Paddle& paddle) {
  if (modifier.applyToTarget(_state, paddle)) {
    notifyModifierApplied(_state, &modifier, &paddle);
    _state.modifiers().eraseObjectById(modifier.id());
    //...?
  }
  //...
}
