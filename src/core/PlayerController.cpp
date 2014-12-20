
#include "PlayerController.h"

// Player connected
void PlayerController::connect(Player& player) {};

// Player has entered the 'lobby'
void PlayerController::queue(Player& player) {};

// Calibrate Player Position
void PlayerController::calibrate(Player& player) {};

// Player has started their game.
void PlayerController::start(Player& player) {};

// Player has quit. Called by player.
void PlayerController::quit(Player& player) {};

// Player's playtime is up. Evaluated at round end.
void PlayerController::expire(Player& player) {};