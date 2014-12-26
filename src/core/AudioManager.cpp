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
#include "BleepoutParameters.h"

AudioManager::AudioManager(const BleepoutParameters& appParams)
: _appParams(appParams) { }

void AudioManager::setup() {
  //...
}

void AudioManager::update() {
  //...
}

void AudioManager::attachTo(RoundController &roundController) {
  ofAddListener(roundController.logicController().brickDestroyedEvent, this, &AudioManager::onBrickDestroyed);
}

void AudioManager::detachFrom(RoundController &roundController) {
  ofRemoveListener(roundController.logicController().brickDestroyedEvent, this, &AudioManager::onBrickDestroyed);
}

void AudioManager::onBrickDestroyed(BrickDestroyedEventArgs &e) {
  _brickDestroyedSound.play();
}
