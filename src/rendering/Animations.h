//
//  Animations.h
//  bleepout
//
//  Created by tekt on 12/14/14.
//
//

#ifndef __bleepout__Animations__
#define __bleepout__Animations__

#include "Common.h"
#include "Timing.h"
#include "GameEvents.h"
#include "GameState.h"
#include <ofTrueTypeFont.h>

class RoundController;

class AnimationManager {
public:
  AnimationManager(RoundController& roundController);
  
  void attachTo(RoundStateEventSource& roundEvents);
  void detachFrom(RoundStateEventSource& roundEvents);
  
  void addMessage(const MessageSpec& message);
  
private:
  void addAnimation(AnimationObject* animation);
  void onBrickDestroyed(BrickDestroyedEventArgs& e);
private:
  RoundController& _roundController;
  ofTrueTypeFont _messageFont;
};

#endif /* defined(__bleepout__Animations__) */
