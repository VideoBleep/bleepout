//
//  RoundResults.h
//  bleepout
//
//  Created by tekt on 12/26/14.
//
//

#ifndef __bleepout__RoundResults__
#define __bleepout__RoundResults__

#include "GameState.h"
#include <vector>
#include <iostream>

enum RoundEndReason {
  END_TIME_LIMIT,
  END_ADMIN_OVERRIDE,
  END_NO_BRICKS,
  END_NO_BALLS,
  END_NO_PLAYERS
};

struct PlayerRoundResult {
  GameObjectId playerId;
  int score;
};

class RoundResults {
public:
  RoundEndReason reason;
  int totalBricks;
  int bricksDestroyed;
  
  void output(std::ostream& os) const;
private:
  std::vector<PlayerRoundResult> _playerResults;
};

#endif /* defined(__bleepout__RoundResults__) */
