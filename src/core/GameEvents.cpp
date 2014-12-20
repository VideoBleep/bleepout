//
//  GameEvents.cpp
//  bleepout
//
//  Created by tekt on 12/4/14.
//
//

#include "GameEvents.h"

void CollisionEventArgs::output(std::ostream &os) const {
  os << "(";
  outputField(os, "a", _a);
  os << " ";
  outputField(os, "b", _b);
  os << ")";
}

void EventSource::logEvent(const char *name,
                           const Outputable &event) const {
  if (loggingEnabled()) {
    ofLog(_logLevel) << "EVENT{" << name << "}: " << event;
  }
}

void CollisionEventSource::notifyCollision(GameObject *a, GameObject *b) {
  CollisionEventArgs e(a, b);
  ofNotifyEvent(collisionEvent, e);
  logEvent("Collision", e);
}

void RoundStateEventSource::notifyBallOwnerChanged(RoundState& state, Ball* ball, Player* player, Player* previousPlayer) {
  BallOwnerChangedEventArgs e(state, ball, player, previousPlayer);
  ofNotifyEvent(ballOwnerChangedEvent, e);
  logEvent("BallOwnerChanged", e);
}
void RoundStateEventSource::notifyBrickDestroyed(RoundState& state, Brick* brick, Ball* ball) {
  BrickDestroyedEventArgs e(state, brick, ball);
  ofNotifyEvent(brickDestroyedEvent, e);
  logEvent("BrickDestroyed", e);
}
void RoundStateEventSource::notifyAllBricksDestroyed(RoundState& state) {
  RoundStateEventArgs e(state);
  ofNotifyEvent(allBricksDestroyedEvent, e);
  logEvent("AllBricksDestroyed", e);
}
void RoundStateEventSource::notifyPlayerScoreChanged(RoundState& state, Player* player) {
  PlayerEventArgs e(state, player);
  ofNotifyEvent(playerScoreChangedEvent, e);
  logEvent("PlayerScoreChanged", e);
}
void RoundStateEventSource::notifyBallDestroyed(RoundState& state, Ball* ball) {
  BallEventArgs e(state, ball);
  ofNotifyEvent(ballDestroyedEvent, e);
  logEvent("BallDestroyed", e);
}
void RoundStateEventSource::notifyBallRespawned(RoundState& state, Ball* ball) {
  BallEventArgs e(state, ball);
  ofNotifyEvent(ballRespawnedEvent, e);
  logEvent("BallRespawned", e);
}
void RoundStateEventSource::notifyPlayerLost(RoundState& state, Player* player) {
  PlayerEventArgs e(state, player);
  ofNotifyEvent(playerLostEvent, e);
  logEvent("PlayerLost", e);
}
void RoundStateEventSource::notifyPlayerLivesChanged(RoundState& state, Player* player) {
  PlayerEventArgs e(state, player);
  ofNotifyEvent(playerLivesChangedEvent, e);
  logEvent("PlayerLivesChanged", e);
}
void RoundStateEventSource::notifyRoundEnded(RoundState& state) {
  RoundStateEventArgs e(state);
  ofNotifyEvent(roundEndedEvent, e);
  logEvent("RoundEnded", e);
}
void RoundStateEventSource::notifyModifierAppeared(RoundState& state, Modifier* modifier, Brick* spawnerBrick) {
  ModifierEventArgs e(state, modifier, spawnerBrick);
  ofNotifyEvent(modifierAppearedEvent, e);
}
void RoundStateEventSource::notifyModifierApplied(RoundState& state, Modifier* modifier, GameObject* target) {
  ModifierEventArgs e(state, modifier, target);
  ofNotifyEvent(modifierAppliedEvent, e);
}
void RoundStateEventSource::notifyModifierRemoved(RoundState& state, Modifier* modifier, GameObject* target) {
  ModifierEventArgs e(state, modifier, target);
  ofNotifyEvent(modifierRemovedEvent, e);
}

void PlayerEventSource::notifyPlayerAdded(RoundState& state, Player* player) {
  PlayerEventArgs e(state, player);
  ofNotifyEvent(playerAddedEvent, e);
  logEvent("PlayerAdded", e);
}
void PlayerEventSource::notifyPlayerRemoved(RoundState& state, Player* player) {
  PlayerEventArgs e(state, player);
  ofNotifyEvent(playerRemovedEvent, e);
  logEvent("PlayerRemoved", e);
}

void SetupEventSource::notifyStartRound(ofPtr<RoundConfig> config,
                      std::list<ofPtr<Player> > players) {
  StartRoundEventArgs e(config, players);
  ofNotifyEvent(startRoundEvent, e);
  logEvent("StartRound", e);
}

void ControlEventSource::notifyPlayerYawPitchRoll(Player* player, float yaw,
                              float pitch, float roll) {
  PlayerYawPitchRollEventArgs e(player, yaw, pitch, roll);
  ofNotifyEvent(playerYawPitchRollEvent, e);
}
