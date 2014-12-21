
#include "PlayerController.h"

// Player connected
void PlayerController::connect(Player& player) {

};

// Player has entered the 'lobby'. Called by PlayerController::configure()
void PlayerController::queue(Player& player) {


};

// Calibrate Player Position
void PlayerController::calibrate(Player& player) {
	/*
		1. Show player a stationary object in the position we believe them to be pointing
		2. Player should now point to the target object with their phone, and click the action button on their screen
		3. TODO: Client should now record compass settings / offset
		4. Once user clicks the action button (calibrate), allow them to control the object and move it to the location they want to play in
		5. Player clicks 'start' once ready
	*/

};

// Player has started their game.
void PlayerController::start(Player& player) {};

// Player has quit. Called by player.
void PlayerController::quit(Player& player) {};

// Player's playtime is up. Evaluated at round end.
void PlayerController::expire(Player& player) {};