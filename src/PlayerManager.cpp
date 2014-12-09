//
//  PlayerManager.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "PlayerManager.h"

PlayerManager::PlayerManager() {
	socket().setup();
}

ofPtr<Player> PlayerManager::addPlayer() {
  ofPtr<Player> player(new Player());
  _players.push_back(player);
  return player;
}
