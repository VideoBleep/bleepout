//
//  BleepoutParameters.h
//  bleepout
//
//  Created by tekt on 12/22/14.
//
//

#ifndef __bleepout__BleepoutParameters__
#define __bleepout__BleepoutParameters__

#include <ofParameterGroup.h>
#include "BleepoutConfig.h"
#include <ofTypes.h>
#include <deque>
#include <string>

// parameters are things that can change, whereas config is fixed
class BleepoutParameters {
public:
  BleepoutParameters(BleepoutConfig& appConfig);
  
  ofParameterGroup& params() { return _params; }
  const BleepoutConfig& appConfig() const { return _appConfig; }
  BleepoutConfig& appConfig() { return _appConfig; }
  const RoundConfig* currentRoundConfig() const { return _currentRoundConfig.get(); }
  std::deque<std::string>& queuedRoundNames() { return _queuedRoundNames; }
  RoundConfig* setCurrentRound(const std::string& name);
  
  RoundConfig* popNextRound();
  
  GameRules& rules() { return _rulesOverrides; }
  const GameRules& rules() const { return _rulesOverrides; }
  
  bool paused;
  bool exitsEnabled;
  bool debugGraphics;
  bool drawTrajectories;
  bool drawComets;
  bool allLasers;
private:
  ofParameterGroup _params;
  BleepoutConfig& _appConfig;
  ofPtr<RoundConfig> _currentRoundConfig;
  std::string _currentRoundName;
  std::deque<std::string> _queuedRoundNames;
  GameRules _rulesOverrides;
};

#endif /* defined(__bleepout__BleepoutParameters__) */
