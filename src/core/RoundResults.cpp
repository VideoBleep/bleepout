//
//  RoundResults.cpp
//  bleepout
//
//  Created by tekt on 12/26/14.
//
//

#include "RoundResults.h"

RoundResults::RoundResults(RoundEndReason r, const RoundState& state)
: reason(r) {
  totalBricks = state.totalBricks();
  liveBricks = state.liveBricks();
  totalBalls = state.totalBalls();
  liveBalls = state.liveBalls();
  duration = state.time;
  
  for (const auto& player : state.players()) {
    PlayerRoundResult presult;
    presult.playerId = player->id();
    presult.score = player->score();
    _playerResults.push_back(presult);
  }
}
