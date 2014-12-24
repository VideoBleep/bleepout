//
//  BleepoutParameters.cpp
//  bleepout
//
//  Created by tekt on 12/22/14.
//
//

#include "BleepoutParameters.h"

BleepoutParameters::BleepoutParameters(BleepoutConfig& appConfig)
: _appConfig(appConfig) {
  for (const auto& round : _appConfig.roundConfigs()) {
    _queuedRoundNames.push_back(round->name());
  }
}

std::string BleepoutParameters::popNextRound() {
  //...
}
