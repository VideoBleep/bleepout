//
//  AudioManager.cpp
//  bleepout
//
//  Created by tekt on 12/26/14.
//
//

#include "AudioManager.h"
#include "GameEvents.h"
#include "RoundManager.h"
#include "BleepoutApp.h"
#include "BleepoutParameters.h"

AudioManager::AudioManager(const BleepoutParameters& appParams)
: _appParams(appParams) { }


static inline void loadSoundSafe(ofSoundPlayer& sound,
                                 const std::string& file) {
  if (!file.empty())
    sound.loadSound(file);
}

void AudioManager::setup() {
  const BleepoutConfig& config = _appParams.appConfig();
  loadSoundSafe(_roundStartedSound, config.roundStartedSound);
  loadSoundSafe(_roundEndedSound, config.roundEndedSound);
  loadSoundSafe(_brickDestroyedSound, config.brickDestroyedSound);
  loadSoundSafe(_collisionSound, config.collisionSound);
  loadSoundSafe(_modifierAppliedSound, config.modifierAppliedSound);
  loadSoundSafe(_modifierRemovedSound, config.modifierRemovedSound);
  loadSoundSafe(_ballDestroyedSound, config.ballDestroyedSound);
  loadSoundSafe(_playerLivesChangedSound, config.playerLivesChangedSound);
  loadSoundSafe(_playerLostSound, config.playerLostSound);
  loadSoundSafe(_countdownTimerTickSound, config.countdownTimerTickSound);
}

void AudioManager::update() {
  _roundStartedSound.setVolume(_appParams.audioVolume);
  _roundEndedSound.setVolume(_appParams.audioVolume);
  _brickDestroyedSound.setVolume(_appParams.audioVolume);
  _collisionSound.setVolume(_appParams.audioVolume);
  _modifierAppliedSound.setVolume(_appParams.audioVolume);
  _modifierRemovedSound.setVolume(_appParams.audioVolume);
  _ballDestroyedSound.setVolume(_appParams.audioVolume);
  _brickDestroyedSound.setVolume(_appParams.audioVolume);
  _playerLivesChangedSound.setVolume(_appParams.audioVolume);
  _playerLostSound.setVolume(_appParams.audioVolume);
  _countdownTimerTickSound.setVolume(_appParams.audioVolume);
}

void AudioManager::attachTo(BleepoutApp &app) {
  ofAddListener(app.roundStartedEvent,
                this, &AudioManager::onRoundStarted);
  ofAddListener(app.roundEndedEvent,
                this, &AudioManager::onRoundEnded);
}

void AudioManager::detachFrom(BleepoutApp &app) {
  ofRemoveListener(app.roundStartedEvent,
                   this, &AudioManager::onRoundStarted);
  ofRemoveListener(app.roundEndedEvent,
                   this, &AudioManager::onRoundEnded);
}

void AudioManager::attachTo(RoundController &roundController) {
  ofAddListener(roundController.logicController().brickDestroyedEvent,
                this, &AudioManager::onBrickDestroyed);
  ofAddListener(roundController.spaceController().collisionEvent,
                this, &AudioManager::onCollision);
  ofAddListener(roundController.logicController().modifierAppliedEvent,
                this, &AudioManager::onModifierApplied);
  ofAddListener(roundController.logicController().modifierRemovedEvent,
                this, &AudioManager::onModifierRemoved);
  ofAddListener(roundController.logicController().ballDestroyedEvent,
                this, &AudioManager::onBallDestroyed);
  ofAddListener(roundController.logicController().playerLivesChangedEvent,
                this, &AudioManager::onPlayerLivesChanged);
  ofAddListener(roundController.logicController().playerLostEvent,
                this, &AudioManager::onPlayerLost);
  ofAddListener(roundController.logicController().countdownTickEvent,
                this, &AudioManager::onCountdownTick);
}

void AudioManager::detachFrom(RoundController &roundController) {
  ofRemoveListener(roundController.logicController().brickDestroyedEvent,
                   this, &AudioManager::onBrickDestroyed);
  ofRemoveListener(roundController.spaceController().collisionEvent,
                   this, &AudioManager::onCollision);
  ofRemoveListener(roundController.logicController().modifierAppliedEvent,
                   this, &AudioManager::onModifierApplied);
  ofRemoveListener(roundController.logicController().modifierRemovedEvent,
                   this, &AudioManager::onModifierRemoved);
  ofRemoveListener(roundController.logicController().ballDestroyedEvent,
                   this, &AudioManager::onBallDestroyed);
  ofRemoveListener(roundController.logicController().playerLivesChangedEvent,
                   this, &AudioManager::onPlayerLivesChanged);
  ofRemoveListener(roundController.logicController().playerLostEvent,
                   this, &AudioManager::onPlayerLost);
  ofRemoveListener(roundController.logicController().countdownTickEvent,
                   this, &AudioManager::onCountdownTick);
}

void AudioManager::onRoundStarted(RoundStateEventArgs &e) {
  _roundStartedSound.play();
}

void AudioManager::onRoundEnded(EmptyEventArgs &e) {
  _roundEndedSound.play();
}

void AudioManager::onBrickDestroyed(BrickDestroyedEventArgs &e) {
  _brickDestroyedSound.play();
}

void AudioManager::onCollision(CollisionEventArgs &e) {
  _collisionSound.play();
}

void AudioManager::onModifierApplied(ModifierEventArgs &e) {
  _modifierAppliedSound.play();
}

void AudioManager::onModifierRemoved(ModifierRemovedEventArgs &e) {
  _modifierRemovedSound.play();
}

void AudioManager::onBallDestroyed(BallStateEventArgs &e) {
  _ballDestroyedSound.play();
}

void AudioManager::onPlayerLivesChanged(PlayerStateEventArgs &e) {
  _playerLivesChangedSound.play();
}

void AudioManager::onPlayerLost(PlayerStateEventArgs &e) {
  _playerLostSound.play();
}

void AudioManager::onCountdownTick(TimerEventArgs &e) {
  _countdownTimerTickSound.play();
}
