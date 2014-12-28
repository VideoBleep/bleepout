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

class BleepoutApp;
class RoundController;
class LogicController;

class AppAnimationManager {
public:
  AppAnimationManager(BleepoutApp& app);
  ~AppAnimationManager();
  
  void addMessage(const MessageSpec& message);
private:
  void addAnimation(AnimationObject* animation);
  
  BleepoutApp& _app;
  ofTrueTypeFont _messageFont;
};

class RoundAnimationManager {
public:
  RoundAnimationManager(RoundController& roundController);
  ~RoundAnimationManager();
  
  void attachTo(LogicController& roundEvents);
  void detachFrom(LogicController& roundEvents);
  
  void addMessage(const MessageSpec& message);
  
private:
  void addAnimation(AnimationObject* animation);
  void onBrickHit(BrickHitEventArgs& e);
  void onModifierApplied(ModifierEventArgs& e);
  void onModifierRemoved(ModifierRemovedEventArgs& e);
  void onCountdownTick(TimerEventArgs& e);
  void onBallSpawned(BallStateEventArgs& e);
private:
  RoundController& _roundController;
  ofTrueTypeFont _messageFont;
};

#endif /* defined(__bleepout__Animations__) */
