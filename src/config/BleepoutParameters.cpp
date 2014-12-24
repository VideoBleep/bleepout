//
//  BleepoutParameters.cpp
//  bleepout
//
//  Created by tekt on 12/22/14.
//
//

#include "BleepoutParameters.h"

BleepoutParameters::BleepoutParameters(BleepoutConfig& appConfig)
: _appConfig(appConfig)
, _paused(false) {
  for (const auto& round : _appConfig.roundConfigs()) {
    _queuedRoundNames.push_back(round->name());
  }
  // yes this is intentional
  for (const auto& round : _appConfig.roundConfigs()) {
    _queuedRoundNames.push_back(round->name());
  }
}

RoundConfig* BleepoutParameters::popNextRound() {
  for (int i = 0; i < _queuedRoundNames.size(); i++) {
    std::string roundName = _queuedRoundNames.front();
    _queuedRoundNames.pop_front();
    _queuedRoundNames.push_back(roundName);
    ofPtr<RoundConfig> round = _appConfig.getRound(roundName);
    if (round) {
      _currentRoundConfig = round;
      _rulesOverrides.setBackup(&round->rules());
      return round.get();
    }
  }
  ofLogError() << "Could not find valid round name";
  return NULL;
}

RoundConfig*
BleepoutParameters::setCurrentRound(const std::string &name) {
  ofPtr<RoundConfig> round = _appConfig.getRound(name);
  if (!round)
    return NULL;
  _currentRoundConfig = round;
  _rulesOverrides.setBackup(&round->rules());
  return round.get();
}
