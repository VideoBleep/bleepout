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

class RoundState;
class RoundConfig;

class AnimationManager {
public:
  AnimationManager(const RoundConfig& config);
  
  void attach(RoundStateEventSource& eventSource);
  
  void draw();
  
private:
  void onBrickDestroyed(BrickDestroyedEventArgs& e);
private:
  const RoundConfig& _config;
  TimedActionSet _animations;
};

#endif /* defined(__bleepout__Animations__) */
