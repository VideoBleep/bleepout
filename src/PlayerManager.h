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

class PlayerManager {
public:
  ofPtr<Player> getPlayer(PlayerId id) {
    return _playerIdMap[id];
  }
  
  std::size_t size() { return _playerList.size(); }
  
  Player& getPlayerAtIndex(std::size_t i) {
    return *(_playerList.at(i).get());
  }
  
private:
  std::map<PlayerId, ofPtr<Player> > _playerIdMap;
  std::vector<ofPtr<Player> > _playerList;
};

#endif /* defined(__bleepout__PlayerManager__) */
