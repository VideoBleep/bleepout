//
//  LogicController.h
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#ifndef __bleepout__LogicController__
#define __bleepout__LogicController__

#include "GameState.h"
#include "GameEvents.h"
#include "BleepoutConfig.h"

class LogicController {
public:
  LogicController(RoundState& state, RoundConfig& config);
  
  void setup();
  void update();
private:
  RoundState& _state;
  RoundConfig& _config;
};

#endif /* defined(__bleepout__LogicController__) */
