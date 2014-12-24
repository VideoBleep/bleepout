//
//  GameEvents.cpp
//  bleepout
//
//  Created by tekt on 12/4/14.
//
//

#include "GameEvents.h"
#include "Logging.h"

void EmptyEventArgs::output(std::ostream &os) const {
  os << "()";
}

void CollisionEventArgs::output(std::ostream &os) const {
  os << "(";
  outputField(os, "a", _a);
  os << " ";
  outputField(os, "b", _b);
  os << ")";
}

void ModifierHitPaddleEventArgs::output(std::ostream &os) const {
  os << "(";
  outputField(os, modifier());
  os << " ";
  outputField(os, paddle());
  os << ")";
}

void RoundStateEventArgs::output(std::ostream &os) const {
  os << "()";
}

void BallOwnerChangedEventArgs::output(std::ostream &os) const {
  os << "(";
  outputField(os, ball());
  os << " ";
  outputField(os, player());
  os << " ";
  outputField(os, "previousPlayer", previousPlayer());
  os << ")";
}

void BrickDestroyedEventArgs::output(std::ostream &os) const {
  os << "(";
  outputField(os, brick());
  os << " ";
  outputField(os, ball());
  os << ")";
}

void ModifierEventArgs::output(std::ostream &os) const {
  os << "(";
  outputField(os, _modifier);
  os << " ";
  outputField(os, "target", _target);
  os << ")";
}

void PlayerStateEventArgs::output(std::ostream &os) const {
  os << "(";
  outputField(os, _player);
  os << ")";
}

void PlayerEventArgs::output(std::ostream &os) const {
  os << "(";
  outputField(os, _player.get());
  os << ")";
}

void BallStateEventArgs::output(std::ostream &os) const {
  os << "(";
  outputField(os, _ball);
  os << ")";
}

void StartRoundEventArgs::output(std::ostream &os) const {
  os << "(";
  os << "config:" << config()->name() << ", ";
  os << "players:" << players().size() << ", ";
  os << "handled:" << std::boolalpha << handled();
  os << ")";
}

void EndRoundEventArgs::output(std::ostream &os) const {
  os << "(";
  os << "handled:" << std::boolalpha << handled();
  os << ")";
}

void ModifierRemovedEventArgs::output(std::ostream &os) const {
  os << "(";
  os << "spec:" << _modifierSpec;
  os << " ";
  outputField(os, "target", _target);
  os << ")";
}

void EventSource::logEvent(const char *name,
                           const Outputable &event) const {
  if (loggingEnabled()) {
    ofLog(_logLevel) << "EVENT{" << name << "}: " << event;
  }
}
