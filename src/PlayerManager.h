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

class PlayerManager {
public:
  ofPtr<Player> getPlayer(PlayerId id) {
    return _players[id];
  }
private:
  std::map<PlayerId, ofPtr<Player> > _players;
};

#endif /* defined(__bleepout__PlayerManager__) */
