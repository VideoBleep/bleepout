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

void ControlEventSource::notifyPlayerYawPitchRoll(Player* player, float yaw,
                              float pitch, float roll) {
  PlayerYawPitchRollEventArgs e(player, yaw, pitch, roll);
  ofNotifyEvent(playerYawPitchRollEvent, e);
}
