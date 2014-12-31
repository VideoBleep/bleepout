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
#include "Timing.h"

class SpaceController;

class LogicController : public EventSource {
public:
  LogicController(RoundState& state);
  virtual ~LogicController() {}
  
  ofEvent<BallOwnerChangedEventArgs> ballOwnerChangedEvent;
  ofEvent<BrickHitEventArgs> brickHitEvent;
  ofEvent<PaddleHitEventArgs> paddleHitEvent;
  ofEvent<WallHitEventArgs> wallHitEvent;
  ofEvent<RoundStateEventArgs> allBricksDestroyedEvent;
  ofEvent<PlayerStateEventArgs > playerScoreChangedEvent;
  ofEvent<BallStateEventArgs> ballDestroyedEvent;
  ofEvent<SpawnBallEventArgs> trySpawnBallEvent;
  ofEvent<PlayerStateEventArgs> playerLostEvent;
  ofEvent<PlayerStateEventArgs> playerLivesChangedEvent;
  ofEvent<EndRoundEventArgs> tryEndRoundEvent;
  ofEvent<ModifierEventArgs> modifierAppearedEvent;
  ofEvent<ModifierEventArgs> modifierDestroyedEvent;
  ofEvent<ModifierEventArgs> modifierAppliedEvent;
  ofEvent<ModifierRemovedEventArgs> modifierRemovedEvent;
  ofEvent<TimerEventArgs> countdownTickEvent;
  
  void setup();
  void resetState();
  void update();
  
  void attachTo(SpaceController& collisions);
  void detachFrom(SpaceController& collisions);
  
  const char* eventSourceName() const override { return "LogicController"; }
  
private:
  void notifyBallOwnerChanged(RoundState& state, Ball* ball, Player* player, Player* previousPlayer);
  void notifyBrickHit(RoundState& state, Brick* brick, Ball* ball);
  void notifyPaddleHit(RoundState& state, Paddle* paddle, Ball* ball);
  void notifyWallHit(RoundState& state, Wall* wall, Ball* ball);
  void notifyAllBricksDestroyed(RoundState& state);
  void notifyPlayerScoreChanged(RoundState& state, Player* player);
  void notifyBallDestroyed(RoundState& state, Ball* ball);
  bool notifyTrySpawnBall(BallSpec ballSpec);
  void notifyPlayerLost(RoundState& state, Player* player);
  void notifyPlayerLivesChanged(RoundState& state, Player* player);
  bool notifyTryEndRound(RoundEndReason reason);
  void notifyModifierAppeared(RoundState& state, Modifier* modifier, Brick* spawnerBrick);
  void notifyModifierDestroyed(RoundState& state, Modifier* modifier);
  void notifyModifierApplied(RoundState& state, Modifier* modifier, GameObject* target);
  void notifyModifierRemoved(RoundState& state, const ModifierSpec &modifierSpec, GameObject* target);
  void notifyCountdownTick();
  
  void onCollision(CollisionEventArgs& e);
  
  void onBallHitObject(Ball& ball, GameObject& object);
  void onModifierHitObject(Modifier& modifier, GameObject& object);
  
  void onBallHitPaddle(Ball& ball, Paddle& paddle);
  void onBallHitBrick(Ball& ball, Brick& brick);
  void onBallHitWall(Ball& ball, Wall& wall);
  void onBallHitBall(Ball& ball, Ball& otherBall);
  void onModifierHitPaddle(Modifier& modifier, Paddle& paddle);
  
  void respawnBall(Player* player);
  
  RoundState& _state;
  float _lastSpecifiedTimeLimitOffset;
  Pulser _countdownTickPulser;
};

#endif /* defined(__bleepout__LogicController__) */
