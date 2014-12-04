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

class PlayerManager {
public:
  GameObjectCollection<Player>& players() { return _players; }
  const GameObjectCollection<Player>& players() const { return _players; }
  
  ofPtr<Player> addPlayer();
private:
  GameObjectCollection<Player> _players;
};

#endif /* defined(__bleepout__PlayerManager__) */
