//
//  BleepoutParameters.cpp
//  bleepout
//
//  Created by tekt on 12/22/14.
//
//

#include "BleepoutParameters.h"

BleepoutParameters BleepoutParameters::_instance;

void BleepoutParameters::initializeConfig(BleepoutConfig &appConfig) {
  _instance.initConfig(appConfig);
}

void BleepoutParameters::initConfig(BleepoutConfig &appConfig) {
  if (_appConfig) {
    ofLogWarning() << "Reinitializing BleepoutParameters config, which is odd and probably a bug";
//    delete _appConfig; ...?
  }
  _appConfig = &appConfig;
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
, enableSyphon(false)
, audioVolume(.5)
, domeRadius(150)
, domeMargin(20)
, minReadyPlayers(2) { }

std::list<ofPtr<RoundConfig> > BleepoutParameters::getRoundQueue() {
  std::list<ofPtr<RoundConfig> > rounds;
  for (auto& round : appConfig().roundConfigs()) {
    rounds.push_back(round);
  }
  return rounds;
}
