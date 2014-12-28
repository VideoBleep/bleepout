//
//  BleepoutParameters.cpp
//  bleepout
//
//  Created by tekt on 12/22/14.
//
//

#include "BleepoutParameters.h"

static ofPtr<BleepoutParameters> instance;

ofPtr<BleepoutParameters> BleepoutParameters::initialize(BleepoutConfig &appConfig) {
  if (instance) {
    ofLogWarning() << "Reinitializing BleepoutParameters, which is odd and probably a bug";
  }
  instance.reset(new BleepoutParameters(appConfig));
  return instance;
}

BleepoutParameters& BleepoutParameters::get() {
  if (!instance) {
    ofLogError() << "BleepoutParameters has not been initialized, which is really bad!";
  }
  return *instance;
}

BleepoutParameters::BleepoutParameters(BleepoutConfig& appConfig)
: _appConfig(appConfig)
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
, syphonAppName(appConfig.syphonAppName())
, syphonServerName(appConfig.syphonServerName())
, audioVolume(.5) {
  for (const auto& round : _appConfig.roundConfigs()) {
    _queuedRoundNames.push_back(round->name());
  }
  // yes this is intentional
  for (const auto& round : _appConfig.roundConfigs()) {
    _queuedRoundNames.push_back(round->name());
  }
}

ofPtr<RoundConfig> BleepoutParameters::popNextRound() {
  for (int i = 0; i < _queuedRoundNames.size(); i++) {
    std::string roundName = _queuedRoundNames.front();
    _queuedRoundNames.pop_front();
    _queuedRoundNames.push_back(roundName);
    ofPtr<RoundConfig> round = _appConfig.getRound(roundName);
    if (round) {
      _currentRoundConfig = round;
      _rulesOverrides.setBackup(&round->rules());
      return round;
    }
  }
  ofLogError() << "Could not find valid round name";
  return ofPtr<RoundConfig>();
}

ofPtr<RoundConfig> BleepoutParameters::getNextRound() {
  const std::string& roundName = _queuedRoundNames.front();
  return _appConfig.getRound(roundName);
}

ofPtr<RoundConfig>
BleepoutParameters::setCurrentRound(const std::string &name) {
  ofPtr<RoundConfig> round = _appConfig.getRound(name);
  if (!round)
    return ofPtr<RoundConfig>();
  _currentRoundConfig = round;
  _rulesOverrides.setBackup(&round->rules());
  return round;
}
