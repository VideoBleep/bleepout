//
//  AudioManager.h
//  bleepout
//
//  Created by tekt on 12/26/14.
//
//

#ifndef __bleepout__AudioManager__
#define __bleepout__AudioManager__

#include <ofSoundPlayer.h>
#include "GameEvents.h"

class BleepoutParameters;
class BleepoutApp;
class RoundController;

class AudioManager {
public:
  AudioManager(const BleepoutParameters& appParams);
  void setup();
  void update();
  void attachTo(BleepoutApp& app);
  void detachFrom(BleepoutApp& app);
  void attachTo(RoundController& roundController);
  void detachFrom(RoundController& roundController);
private:
  void onRoundStarted(RoundStateEventArgs& e);
  void onRoundEnded(EmptyEventArgs& e);
  void onBrickHit(BrickHitEventArgs& e);
  void onCollision(CollisionEventArgs& e);
  void onModifierApplied(ModifierEventArgs& e);
  void onModifierRemoved(ModifierRemovedEventArgs& e);
  void onBallDestroyed(BallStateEventArgs& e);
  void onPlayerLivesChanged(PlayerStateEventArgs& e);
  void onPlayerLost(PlayerStateEventArgs& e);
  void onCountdownTick(TimerEventArgs& e);
  
  const BleepoutParameters& _appParams;
  ofSoundPlayer _roundStartedSound;
  ofSoundPlayer _roundEndedSound;
  ofSoundPlayer _brickDestroyedSound;
  ofSoundPlayer _brickHitSound;
  ofSoundPlayer _collisionSound;
  ofSoundPlayer _modifierAppliedSound;
  ofSoundPlayer _modifierRemovedSound;
  ofSoundPlayer _ballDestroyedSound;
  ofSoundPlayer _playerLivesChangedSound;
  ofSoundPlayer _playerLostSound;
  ofSoundPlayer _countdownTimerTickSound;
};

#endif /* defined(__bleepout__AudioManager__) */
