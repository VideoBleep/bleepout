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
  SetupController();
  virtual ~SetupController() {}
  void setup();
  void update();
  bool canStartRound() const;
  
  ofEvent<StartRoundEventArgs> tryStartRoundEvent;
  
  const char* eventSourceName() const override { return "SetupController"; }
  
  std::list<ofPtr<Player> >& lobby() { return _lobby; }

  // Event Handlers
  void handlePlayerConnected(PlayerEventArgs& e);

private:

	// Lobby is the list of players queued for the game
	std::list<ofPtr<Player> > _lobby;
};

#endif /* defined(__bleepout__SetupController__) */
