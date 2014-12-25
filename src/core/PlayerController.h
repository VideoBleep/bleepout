/*
	PlayerController
	By Jim Ankrom

	PlayerController coordinates player actions and state
	
	Actions are routed to the controller from messages recieved by PlayerManager

	The player lifecycle is as follows:

	* Connect
	* Configure
	* Queue
	* Calibrate
	* 
	

*/
#include "Player.h"
#include "GameEvents.h"
//#include "SetupController.h"

#ifndef __bleepout__PlayerController__
#define __bleepout__PlayerController__

//class SeutupController;

class PlayerController : public EventSource
{
public:
	PlayerController();  // (SetupController& setup);

	/*
		Player Actions
	*/
	// Player connected - called by message from player
	void connect(ofPtr<Player> player);
	// Player configuring - called by message from player
	void configure(ofPtr<Player> player, vector<string> parts); // , std::string[] & messageParts);
	// Player has entered the 'lobby' - called by player controller itself
	void queue(ofPtr<Player> player);
	// Calibrate Player Position - called by player, but this likely is a complicated step
	void calibrate(ofPtr<Player> player);
	// Player has started their game - called by player
	void start(ofPtr<Player> player);
	// Player has quit. Called by player.
	void quit(ofPtr<Player> player);
	// Player's playtime is up. Evaluated at round end.
	void expire(ofPtr<Player> player);

	void onRoundQueue(RoundStateEventArgs& e); 
	/*
		Events
	*/
	// Raised when player connects initially
	ofEvent<PlayerEventArgs> playerConnectedEvent;
	// ?
	ofEvent<PlayerEventArgs> playerAddedEvent;
	// ?
	ofEvent<PlayerEventArgs> playerRemovedEvent;
  
  const char* eventSourceName() const { return "PlayerController"; }

private:
	//SetupController& _setup;

	void notifyPlayerConnected(ofPtr<Player> player);
	void notifyPlayerAdded(ofPtr<Player> player);
	void notifyPlayerRemoved(ofPtr<Player> player);
};

#endif /* defined(__bleepout__PlayerController__) */