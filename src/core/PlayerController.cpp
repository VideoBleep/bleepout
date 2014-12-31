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

	1. [CONNECT] Client - New Player > PlayerController::connect
  - New player connects by sending socket message to Bleepout.
  - Bleepout responds with handshake (PlayerManager
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

#include "PlayerController.h"
#include "PlayerManager.h"
#include "GameEvents.h"
#include "Logging.h"
#include "AdminController.h"

PlayerController::PlayerController(AdminController& setup)
	: _setup(setup) 
{

}

// Player connected
// 1.[CONNECT] Client - New Player > PlayerController::connect
void PlayerController::connect(ofPtr<Player> player) {
	_setup.lobby().push_back(player);
	
	// TODO: Set player state to 'select color'
  // Send message to player that they are in the select color state
  // 2.[SETCOLOR] PlayerManager::setPlayerColor < Select Color -
  //PlayerManager::setPlayerColor(player, ofColor::red);

	// TODO: Remove this... For now: Set player to Queued. 
  this->queue(*(player.get()));
};

// Player has entered the 'lobby'. Called by PlayerController::configure()
void PlayerController::queue(Player& player) {
  // 4.[QUEUED] PlayerManager::setPlayerQueued < Queued -
  PlayerManager::setPlayerQueued(player);
};

// Calibrate Player Position
//5.[SETCALIBRATE] PlayerManager::setPlayerCalibrate < Calibrate -
void PlayerController::calibrate(Player& player) {
	/*
		1. Show player a stationary object in the position we believe them to be pointing
		2. Player should now point to the target object with their phone, and click the action button on their screen
		3. TODO: Client should now record compass settings / offset

    //5.[SETCALIBRATE] PlayerManager::setPlayerCalibrate < Calibrate -
		4. Once user clicks the action button (calibrate), allow them to control the object and move it to the location they want to play in
		5. Player clicks 'start' once ready
	*/
  //PlayerManager::setPlayerCalibrate(player);
};

// Player is calibrated
void PlayerController::setCalibrate(Player& player) {
  // This sets GAME READY, not "Player Ready"
  PlayerManager::setPlayerReady(player);
};

// Handle configure message from player (set color)
// 3. [CONFIGURED] Client - Set Color > PlayerController::configure
void PlayerController::configure(Player& player, ofColor color) {
  player.setColor(color);

  // set player queued
  this->queue(player);
};

// Player has started their game.
void PlayerController::start(Player& player) {
  notifyPlayerReady(player);
};

// Player has quit. Called by player.
void PlayerController::quit(Player& player) {
	// TODO: What happens here? Delete the paddle? Set player state to quit?
};

// Player's playtime is up. Evaluated at round end.
void PlayerController::expire(Player& player) {
	// Should be similar to quit
};

// Events
void PlayerController::notifyPlayerConnected(Player& player) {
	PlayerEventArgs e(&player);
	ofNotifyEvent(playerConnectedEvent, e);
	logEvent("PlayerConnected", e);
}

void PlayerController::notifyPlayerReady(Player& player) {
	PlayerEventArgs e(&player);
	ofNotifyEvent(playerReadyEvent, e);
	logEvent("PlayerReady", e);
}
	
void PlayerController::notifyPlayerStart(Player& player) {
	PlayerEventArgs e(&player);
	ofNotifyEvent(playerStartEvent, e);
	logEvent("PlayerStart", e);
}

void PlayerController::notifyPlayerQuit(Player& player) {
	PlayerEventArgs e(&player);
	ofNotifyEvent(playerQuitEvent, e);
	logEvent("PlayerQuit", e);
}
