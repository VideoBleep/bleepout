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
