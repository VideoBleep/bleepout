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

#ifndef __bleepout__BleepoutApp__
#define __bleepout__BleepoutApp__

class PlayerController
{
public: 
	/*
		Player Actions

	*/
	// Player connected - called by message from player
	void connect(Player& player);
	// Player configuring - called by message from player
	void configure(Player& player);
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
};

#endif /* defined(__bleepout__BleepoutApp__) */