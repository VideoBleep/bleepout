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
private:
  static BleepoutParameters *_instance;
  BleepoutParameters(void);
  void initConfig(BleepoutConfig& appConfig);
public:
  static void initialize();
  static void initializeConfig(BleepoutConfig& appConfig);
  static void cleanup();
  static inline BleepoutParameters& get() {
    return *_instance;
  }
  
  ofParameterGroup& params() { return _params; }
  const BleepoutConfig& appConfig() const { return *_appConfig; }
  BleepoutConfig& appConfig() { return *_appConfig; }
  const RoundConfig* currentRoundConfig() const { return _currentRoundConfig.get(); }
  std::deque<std::string>& queuedRoundNames() { return _queuedRoundNames; }
  ofPtr<RoundConfig> setCurrentRound(const std::string& name);
  ofPtr<RoundConfig> popNextRound();
  ofPtr<RoundConfig> getNextRound();
  
  GameRules& rules() { return _rulesOverrides; }
  const GameRules& rules() const { return _rulesOverrides; }
  
  bool inRound;
  bool paused;
  bool exitsEnabled;
  bool debugGraphics;
  bool drawTrajectories;
  bool drawComets;
  bool drawExtras;
  bool allLasers;
  int ballsToAdd;
  bool enableSyphon;
  std::string syphonAppName;
  std::string syphonServerName;
  float audioVolume;
  float domeRadius;
  float domeMargin;
private:
  ofParameterGroup _params;
  BleepoutConfig* _appConfig;
  ofPtr<RoundConfig> _currentRoundConfig;
  std::string _currentRoundName;
  std::deque<std::string> _queuedRoundNames;
  GameRules _rulesOverrides;
};

#endif /* defined(__bleepout__BleepoutParameters__) */
