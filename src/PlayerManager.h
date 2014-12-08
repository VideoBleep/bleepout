//
//  PlayerManager.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__PlayerManager__
#define __bleepout__PlayerManager__

#include "Player.h"
#include <ofMain.h>
#include <map>
#include <vector>
#include "GameObjectCollection.h"
#include "PlayerSocket.h"

class PlayerManager {
public:
	PlayerManager();

  GameObjectCollection<Player>& players() { return _players; }
  const GameObjectCollection<Player>& players() const { return _players; }
  PlayerSocket& socket() { return _socket; }

  ofPtr<Player> addPlayer();
private:
  GameObjectCollection<Player> _players;
  PlayerSocket _socket;
};

#endif /* defined(__bleepout__PlayerManager__) */
