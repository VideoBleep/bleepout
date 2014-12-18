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
#include "PlayerManager.h"
#include <list>
#include <ofTypes.h>

class SetupController : public SetupEventSource {
public:
  SetupController(const BleepoutConfig& appConfig);
  void setup();
  void update();
  void draw();
  void keyPressed(int key);
  
private:
  bool tryStartRound();
  bool canStartRound() const;
  
  ofPtr<PlayerManager> _playerManager;
  const BleepoutConfig& _appConfig;
  std::list<ofPtr<Player> > _players;
  ofPtr<RoundConfig> _roundConfig;
};

#endif /* defined(__bleepout__SetupController__) */
