/*
	PlayerController.cpp
	
	By Jim Ankrom
	
	PlayerController coordinates player actions and state
	
	Actions are routed to the controller from messages recieved by PlayerManager

	The player lifecycle is as follows:

	* Connect
	* Configure
	* Queue
	* Calibrate


	Message Exchange Sequence 

	Constant:

	- New Player > PlayerController::connect
	PlayerManager::setPlayerColor < Select Color - 
	- Set Color > PlayerController::configure
	PlayerManager::setPlayerQueued < Queued -

	Round Queue:

	PlayerManager::setPlayerCalibrate < Calibrate -
	- Calibration Complete > PlayerController::calibrate
	< Game Ready -
	- Player Start > PlayerController::start


	Round Play:

	< Play -
	- Player Control > PlayerController::control  (?)
	- Quit > PlayerController::quit
*/

#include "PlayerController.h"
#include "PlayerManager.h"
#include "GameEvents.h"
#include "Logging.h"

PlayerController::PlayerController() //(SetupController& setup)
	//: _setup(setup) 
{

}

// Player connected
void PlayerController::connect(ofPtr<Player> player) {
	//_setup.Lobby().push_back(player);
	// TODO: Set player state to 'select color'
	// TODO: send message to player that they are in that state

	// For now: Set player to Queued
	//PlayerManager::setPlayerQueued(player);
};

// Player has entered the 'lobby'. Called by PlayerController::configure()
void PlayerController::queue(ofPtr<Player>  player) {

};

// Calibrate Player Position
void PlayerController::calibrate(ofPtr<Player> player) {
	/*
		1. Show player a stationary object in the position we believe them to be pointing
		2. Player should now point to the target object with their phone, and click the action button on their screen
		3. TODO: Client should now record compass settings / offset
		4. Once user clicks the action button (calibrate), allow them to control the object and move it to the location they want to play in
		5. Player clicks 'start' once ready
	*/
};


// Handle configure message from player (set color)
void PlayerController::configure(ofPtr<Player> player, vector<string> parts) {

};

// Player has started their game.
void PlayerController::start(ofPtr<Player> player) {
	// TODO: implement
};

// Player has quit. Called by player.
void PlayerController::quit(ofPtr<Player> player) {};

// Player's playtime is up. Evaluated at round end.
void PlayerController::expire(ofPtr<Player> player) {};

void PlayerController::onRoundQueue(RoundStateEventArgs& e) {
	// iterate each of the players in the game queue (not lobby) and notify them to begin calibrate
	
}

// Events
void PlayerController::notifyPlayerConnected(ofPtr<Player> player) {
	PlayerEventArgs e(player);
	ofNotifyEvent(playerConnectedEvent, e);
	logEvent("PlayerConnected", e);
}


void PlayerController::notifyPlayerAdded(ofPtr<Player> player) {
	PlayerEventArgs e(player);
	ofNotifyEvent(playerAddedEvent, e);
	logEvent("PlayerAdded", e);
}

void PlayerController::notifyPlayerRemoved(ofPtr<Player> player) {
	PlayerEventArgs e(player);
	ofNotifyEvent(playerRemovedEvent, e);
	logEvent("PlayerRemoved", e);
}
