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
	
  Message Exchange Sequence

  Constant:

  1. [CONNECT] Client - New Player > PlayerController::connect
  2. [SETCOLOR] PlayerManager::setPlayerColor < Select Color -
  3. [CONFIGURED] Client - Set Color > PlayerController::configure
  4. [QUEUED] PlayerManager::setPlayerQueued < Queued -

  Round Queue:

  5. [SETCALIBRATE] PlayerManager::setPlayerCalibrate < Calibrate -
  6. [CALIBRATED] Client - Calibration Complete > PlayerController::calibrate
  7. [GAME READY] PlayerManager::setPlayer???? < Game Ready -
  8. [PLAYER START] Client - Player Start > PlayerController::start


  Round Play:

  9. [PLAYER PLAY] < Play -
  10. [CONTROL] Client - Player Control > PlayerController::control  (?)
  11. [QUIT] Client - Quit > PlayerController::quit
*/
#ifndef __bleepout__PlayerController__
#define __bleepout__PlayerController__

#include "SetupController.h"
#include "Player.h"
#include "GameEvents.h"

class PlayerController : public EventSource
{
public:
  PlayerController(SetupController& setup);

	/*
		Player Actions
	*/
	// Player connected - called by message from player
  void connect(Player& player);
	// Player configuring - called by message from player
  void configure(Player& player, ofColor color); // , std::string[] & messageParts);
	// Player has entered the 'lobby' - called by player controller itself
	void queue(Player& player);
	// Calibrate Player Position - called by player, but this likely is a complicated step
	void calibrate(Player& player);
	// Player has started their game - called by player
	void start(Player& player);
	// Player has quit. Called by player.
	void quit(Player& player);
	// Player's playtime is up. Evaluated at round end.
	void expire(Player& player);

  /*
		Events
	*/
	// Raised when player connects initially
	ofEvent<PlayerEventArgs> playerConnectedEvent;
	ofEvent<PlayerEventArgs> playerReadyEvent;
	ofEvent<PlayerEventArgs> playerStartEvent;
	ofEvent<PlayerEventArgs> playerQuitEvent;

  const char* eventSourceName() const { return "PlayerController"; }

private:
  SetupController& _setup;

	void notifyPlayerConnected(Player& player);
	void notifyPlayerReady(Player& player);
	void notifyPlayerStart(Player& player);
	void notifyPlayerQuit(Player& player);
};

#endif /* defined(__bleepout__PlayerController__) */