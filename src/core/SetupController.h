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
#include <list>
#include <ofTypes.h>

//class PlayerManager;

class SetupController : public EventSource {
public:
  SetupController(const BleepoutConfig& appConfig);
  void setup();
  void update();
  void draw();
  void keyPressed(int key);
  bool canStartRound() const;
  
  ofEvent<StartRoundEventArgs> tryStartRoundEvent;
  
  const char* eventSourceName() const override { return "SetupController"; }
  
  std::list<ofPtr<Player> >& lobby() { return _lobby; }
  
  // Event Handlers
  void handlePlayerConnected(PlayerEventArgs& e);
  
private:
  bool notifyTryStartRound(ofPtr<RoundConfig> config,
                           std::list<ofPtr<Player> > players);
  bool tryStartRound();
  
  //ofPtr<PlayerManager> _playerManager;
  // Lobby is the list of players queued for the game
  std::list<ofPtr<Player> > _lobby;
  // ConnectedPlayers is all current players
  std::list<ofPtr<Player> > _connectedPlayers;
  
  const BleepoutConfig& _appConfig;
  ofPtr<RoundConfig> _roundConfig;
};

#endif /* defined(__bleepout__SetupController__) */
