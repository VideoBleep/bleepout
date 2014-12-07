//
//  GameState.cpp
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#include "GameState.h"

void RoundState::dumpToLog() {
  ofLogVerbose() << "--BEGIN round state--";
  _paddles.dumpToLog("Paddles");
  _balls.dumpToLog("Balls");
  _bricks.dumpToLog("Bricks");
  _players.dumpToLog("Players");
  ofLogVerbose() << "--  END round state--";
}
