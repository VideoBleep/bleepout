//
//  BleepoutParameters.cpp
//  bleepout
//
//  Created by tekt on 12/22/14.
//
//

#include "BleepoutParameters.h"

BleepoutParameters* BleepoutParameters::_instance = NULL;

void BleepoutParameters::initialize() {
  if (_instance) {
    ofLogWarning() << "Reinitializing BleepoutParameters, which is odd and probably a bug";
//    delete _instance; ...?
  }
  _instance = new BleepoutParameters();
}

void BleepoutParameters::initializeConfig(BleepoutConfig &appConfig) {
  _instance->initConfig(appConfig);
}

void BleepoutParameters::cleanup() {
  if (_instance) {
    delete _instance;
    _instance = NULL;
  }
}

void BleepoutParameters::initConfig(BleepoutConfig &appConfig) {
  if (_appConfig) {
    ofLogWarning() << "Reinitializing BleepoutParameters config, which is odd and probably a bug";
//    delete _appConfig; ...?
  }
  _appConfig = &appConfig;
  _queuedRoundNames.clear();
  for (const auto& round : appConfig.roundConfigs()) {
    _queuedRoundNames.push_back(round->name());
  }
  syphonAppName = appConfig.syphonAppName();
  syphonServerName = appConfig.syphonServerName();
}

BleepoutParameters::BleepoutParameters(void)
: _appConfig(NULL)
, paused(false)
, inRound(false)
, exitsEnabled(false)
, debugGraphics(false)
, drawTrajectories(false)
, drawComets(false)
, drawExtras(true)
, allLasers(false)
, ballsToAdd(0)
, enableSyphon(true)
, audioVolume(.5)
, domeRadius(150)
, domeMargin(20)
, minReadyPlayers(2) { }

std::list<ofPtr<RoundConfig> > BleepoutParameters::getRoundQueue() {
  std::list<ofPtr<RoundConfig> > rounds;
  for (const auto& name : _queuedRoundNames) {
    ofPtr<RoundConfig> round = appConfig().getRound(name);
    if (round)
      rounds.push_back(round);
  }
  return rounds;
}
