//
//  GameState.cpp
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#include "GameState.h"

void GameState::dumpToLog() {
  ofLogVerbose() << "--BEGIN game state--";
  _paddles.dumpToLog("Paddles");
  _balls.dumpToLog("Balls");
  _bricks.dumpToLog("Bricks");
  ofLogVerbose() << "--  END game state--";
}
