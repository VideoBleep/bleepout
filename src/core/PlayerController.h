/*
	PlayerController
	By Jim Ankrom

	PlayerController coordinates player actions and state
	
	Actions are routed to the controller from messages recieved by PlayerManager

*/
#include "Player.h"

#ifndef __bleepout__BleepoutApp__
#define __bleepout__BleepoutApp__

class PlayerController
{
public: 
	/*
		Player Actions
	*/
	// Player connected
	void connect(Player& player);
	// Player has entered the 'lobby'
	void queue(Player& player);
	// Calibrate Player Position
	void calibrate(Player& player);
	// Player has started their game.
	void start(Player& player);
	// Player has quit. Called by player.
	void quit(Player& player);
	// Player's playtime is up. Evaluated at round end.
	void expire(Player& player);
};

#endif /* defined(__bleepout__BleepoutApp__) */