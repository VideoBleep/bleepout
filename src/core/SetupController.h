//
//  SetupController.h
//  bleepout
//
//  Created by tekt on 12/16/14.
//
//

#ifndef __bleepout__SetupController__
#define __bleepout__SetupController__

#include "Player.h"
#include "BleepoutConfig.h"
#include "GameEvents.h"
#include <vector>
#include <ofTypes.h>

class RoundController;

class SetupController {
public:
  SetupController(const BleepoutConfig& appConfig);
  void setup();
  void update();
  void draw();
  bool tryStartRound(ofPtr<RoundController>& roundController);
private:
  const BleepoutConfig& _appConfig;
  std::vector<Player> _players;
  ofPtr<RoundConfig> _roundConfig;
};

#endif /* defined(__bleepout__SetupController__) */
