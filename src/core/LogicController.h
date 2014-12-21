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

class SpaceController;

class LogicController : public EventSource {
public:
  LogicController(RoundState& state, RoundConfig& config);
  
  ofEvent<BallOwnerChangedEventArgs> ballOwnerChangedEvent;
  ofEvent<BrickDestroyedEventArgs> brickDestroyedEvent;
  ofEvent<RoundStateEventArgs> allBricksDestroyedEvent;
  ofEvent<PlayerStateEventArgs > playerScoreChangedEvent;
  ofEvent<BallStateEventArgs> ballDestroyedEvent;
  ofEvent<BallStateEventArgs> ballRespawnedEvent;
  ofEvent<PlayerStateEventArgs> playerLostEvent;
  ofEvent<PlayerStateEventArgs> playerLivesChangedEvent;
  ofEvent<RoundStateEventArgs> roundEndedEvent;
  ofEvent<ModifierEventArgs> modifierAppearedEvent;
  ofEvent<ModifierEventArgs> modifierAppliedEvent;
  ofEvent<ModifierEventArgs> modifierRemovedEvent;
  
  void setup();
  void update();
  
  void attachTo(SpaceController& collisions);
  void detachFrom(SpaceController& collisions);
  
private:
  void notifyBallOwnerChanged(RoundState& state, Ball* ball, Player* player, Player* previousPlayer);
  void notifyBrickDestroyed(RoundState& state, Brick* brick, Ball* ball);
  void notifyAllBricksDestroyed(RoundState& state);
  void notifyPlayerScoreChanged(RoundState& state, Player* player);
  void notifyBallDestroyed(RoundState& state, Ball* ball);
  void notifyBallRespawned(RoundState& state, Ball* ball);
  void notifyPlayerLost(RoundState& state, Player* player);
  void notifyPlayerLivesChanged(RoundState& state, Player* player);
  void notifyRoundEnded(RoundState& state);
  void notifyModifierAppeared(RoundState& state, Modifier* modifier, Brick* spawnerBrick);
  void notifyModifierApplied(RoundState& state, Modifier* modifier, GameObject* target);
  void notifyModifierRemoved(RoundState& state, Modifier* modifier, GameObject* target);
  
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
