//
//  BleepoutParameters.h
//  bleepout
//
//  Created by tekt on 12/22/14.
//
//

#ifndef __bleepout__BleepoutParameters__
#define __bleepout__BleepoutParameters__

#include "BleepoutConfig.h"
#include <ofTypes.h>
#include <deque>
#include <string>
#include <list>

// parameters are things that can change, whereas config is fixed
class BleepoutParameters {
private:
  static BleepoutParameters _instance;
  BleepoutParameters(void);
  void initConfig(BleepoutConfig& appConfig);
public:
  static void initializeConfig(BleepoutConfig& appConfig);
  static inline BleepoutParameters& get() {
    return _instance;
  }
  
  const BleepoutConfig& appConfig() const { return *_appConfig; }
  BleepoutConfig& appConfig() { return *_appConfig; }
  
  std::list<ofPtr<RoundConfig> > getRoundQueue();
  
  GameRules& rules() { return _rulesOverrides; }
  const GameRules& rules() const { return _rulesOverrides; }
  
  bool inRound;
  bool paused;
  bool exitsEnabled;
  bool ballsRespawn;
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
  int minReadyPlayers;
private:
  BleepoutConfig* _appConfig;
  std::string _currentRoundName;
  GameRules _rulesOverrides;
};

#endif /* defined(__bleepout__BleepoutParameters__) */
