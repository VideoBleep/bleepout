//
//  LogicController.cpp
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#include "LogicController.h"
#include "SpaceController.h"
#include "BleepoutParameters.h"

LogicController::LogicController(RoundState& state)
:_state(state)
, _lastSpecifiedTimeLimitOffset(-1), _countdownTickPulser(1)
, EventSource() { }

void LogicController::setup() {
  
}

void LogicController::resetState() {
  _lastSpecifiedTimeLimitOffset = -1;
}

void LogicController::attachTo(SpaceController &collisions) {
  ofAddListener(collisions.collisionEvent, this,
                &LogicController::onCollision);
}

void LogicController::detachFrom(SpaceController &collisions) {
  ofRemoveListener(collisions.collisionEvent, this,
                   &LogicController::onCollision);
}

void LogicController::update() {
  float limit = BleepoutParameters::get().rules().timeLimit();
  if (limit != _lastSpecifiedTimeLimitOffset) {
    if (limit == -1) {
      _state.endTime = -1;
    } else {
      _state.endTime = _state.time + limit;
    }
    _lastSpecifiedTimeLimitOffset = limit;
  }
  if (_state.endTime > 0) {
    if (_state.remainingTime() <= _state.config().countdownTimerPeriod &&
        _countdownTickPulser.update(_state.time))
      notifyCountdownTick();
    if (_state.remainingTime() <= 0) {
      notifyTryEndRound(END_TIME_LIMIT);
      return;
    }
  }
  for (auto& obj : _state.paddles()) {
    if (obj && obj->alive()) {
      const ofPtr<ModifierSpec> mod = obj->updateWidthModifier(_state);
      if (mod) {
        notifyModifierRemoved(_state, *mod, obj.get());
      }
    }
  }
  for (auto& obj : _state.balls()) {
    if (obj && obj->alive()) {
      const ofPtr<ModifierSpec> mod = obj->updateModifier(_state);
      if (mod) {
        notifyModifierRemoved(_state, *mod, obj.get());
      }
    }
  }
  std::vector<ofPtr<Modifier> > deadModifiers;
  for (auto& obj : _state.modifiers()) {
    if (obj && obj->alive()) {
      if (obj->getPosition().y <= 0) {
        obj->kill();
      }
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
  Player& player = paddle.player();
  ball.setPlayer(&player);
  if (previousPlayer == NULL ||
      player.id() != previousPlayer->id()) {
    notifyBallOwnerChanged(_state, &ball, &player, previousPlayer);
  }
  ModifierSpec modifierSpec;
  if (player.tryDequeueBallModifier(&modifierSpec)) {
    ball.applyModifier(_state, modifierSpec);
    //... event?
  }
}

void LogicController::onBallHitBrick(Ball& ball, Brick& brick) {
  Player* player = ball.player();
  if (player) {
    brick.adjustLives(-1);
    if (!brick.alive()) {
      brick.kill();
      _state.decrementLiveBricks();
      
      const std::string& modifierName = brick.modifierName();
      if (!modifierName.empty()) {
        const ModifierSpec& spec = _state.config().modifierDef(modifierName);
        ofPtr<Modifier> modifier(Modifier::createModifier(spec));
        if (modifier) {
          modifier->setup(_state.config(), brick);
          _state.addModifier(modifier);
          notifyModifierAppeared(_state, modifier.get(), &brick);
        }
      }
      
      player->adjustScore(brick.value());
      notifyPlayerScoreChanged(_state, player);
      
      if (_state.liveBricks() <= 0) {
        notifyAllBricksDestroyed(_state);
        notifyTryEndRound(END_NO_BRICKS);
      }
    }
  }
  notifyBrickHit(_state, &brick, &ball);
}

void LogicController::onBallHitWall(Ball& ball, Wall& wall) {
  if (wall.isExit() && BleepoutParameters::get().exitsEnabled) {
    Player* player = ball.player();
    
    ball.kill();
    _state.decrementLiveBalls();
    notifyBallDestroyed(_state, &ball);
    
    if (player && BleepoutParameters::get().rules().playersCanLoseLives()) {
      player->adjustLives(-1);
      notifyPlayerLivesChanged(_state, player);
      if (!player->alive()) {
        notifyPlayerLost(_state, player);
      }
    }
    if (BleepoutParameters::get().rules().ballsRespawn()) {
      respawnBall(player);
    }
    
    if (_state.liveBalls() <= 0) {
      notifyTryEndRound(END_NO_BALLS);
    }
  }
}

void LogicController::respawnBall(Player *player) {
  Paddle* paddle = player ? player->paddle() : NULL;
  BallSpec spec;
  spec.elevation = 30;
  spec.heading = ofRandom(360);
  if (paddle) {
//    spec.heading = paddle->
  } else {
    
  }
  notifyTrySpawnBall(spec);
}

void LogicController::onBallHitBall(Ball& ball, Ball& otherBall) {
  //...
}

void LogicController::onModifierHitPaddle(Modifier& modifier, Paddle& paddle) {
  if (modifier.applyToTarget(_state, paddle)) {
    notifyModifierApplied(_state, &modifier, &paddle);
  }
  modifier.kill();
}

void LogicController::notifyBallOwnerChanged(RoundState& state, Ball* ball, Player* player, Player* previousPlayer) {
  BallOwnerChangedEventArgs e(state, ball, player, previousPlayer);
  ofNotifyEvent(ballOwnerChangedEvent, e);
  logEvent("BallOwnerChanged", e);
}
void LogicController::notifyBrickHit(RoundState& state, Brick* brick, Ball* ball) {
  BrickHitEventArgs e(state, brick, ball);
  ofNotifyEvent(brickHitEvent, e);
  logEvent("BrickHit", e);
}
void LogicController::notifyAllBricksDestroyed(RoundState& state) {
  RoundStateEventArgs e(state);
  ofNotifyEvent(allBricksDestroyedEvent, e);
  logEvent("AllBricksDestroyed", e);
}
void LogicController::notifyPlayerScoreChanged(RoundState& state, Player* player) {
  PlayerStateEventArgs e(state, player);
  ofNotifyEvent(playerScoreChangedEvent, e);
  logEvent("PlayerScoreChanged", e);
}
void LogicController::notifyBallDestroyed(RoundState& state, Ball* ball) {
  BallStateEventArgs e(state, ball);
  ofNotifyEvent(ballDestroyedEvent, e);
  logEvent("BallDestroyed", e);
}
bool LogicController::notifyTrySpawnBall(BallSpec ballSpec) {
  SpawnBallEventArgs e(ballSpec);
  logEvent("TrySpawnBall", e);
  ofNotifyEvent(trySpawnBallEvent, e);
  return e.handled();
}
void LogicController::notifyPlayerLost(RoundState& state, Player* player) {
  PlayerStateEventArgs e(state, player);
  ofNotifyEvent(playerLostEvent, e);
  logEvent("PlayerLost", e);
}
void LogicController::notifyPlayerLivesChanged(RoundState& state, Player* player) {
  PlayerStateEventArgs e(state, player);
  ofNotifyEvent(playerLivesChangedEvent, e);
  logEvent("PlayerLivesChanged", e);
}
bool LogicController::notifyTryEndRound(RoundEndReason reason) {
  EndRoundEventArgs e(reason);
  ofNotifyEvent(tryEndRoundEvent, e);
  logEvent("TryEndRound", e);
  return e.handled();
}
void LogicController::notifyModifierAppeared(RoundState& state, Modifier* modifier, Brick* spawnerBrick) {
  ModifierEventArgs e(state, modifier, spawnerBrick);
  ofNotifyEvent(modifierAppearedEvent, e);
  logEvent("ModifierAppeared", e);
}
void LogicController::notifyModifierDestroyed(RoundState &state, Modifier *modifier) {
  ModifierEventArgs e(state, modifier, NULL);
  ofNotifyEvent(modifierDestroyedEvent, e);
  logEvent("ModifierDestroyed", e);
}
void LogicController::notifyModifierApplied(RoundState& state, Modifier* modifier, GameObject* target) {
  ModifierEventArgs e(state, modifier, target);
  ofNotifyEvent(modifierAppliedEvent, e);
  logEvent("ModifierApplied", e);
}
void LogicController::notifyModifierRemoved(RoundState& state, const ModifierSpec& modifierSpec, GameObject* target) {
  ModifierRemovedEventArgs e(state, modifierSpec, target);
  ofNotifyEvent(modifierRemovedEvent, e);
  logEvent("ModifierRemoved", e);
}
void LogicController::notifyCountdownTick() {
  TimerEventArgs e(_state.time, _state.remainingTime());
  ofNotifyEvent(countdownTickEvent, e);
  logEvent("CountdownTick", e);
}
