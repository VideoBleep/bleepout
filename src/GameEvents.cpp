//
//  GameEvents.cpp
//  bleepout
//
//  Created by tekt on 12/4/14.
//
//

#include "GameEvents.h"
#include <ofMain.h>


class CollisionLogger {
public:
  CollisionLogger(ofLogLevel level) : _level(level) { }

  void onBallHitPaddle(BallHitPaddleEventArgs& e) {
    ofLog(_level) << "EVENT: ballHitPaddle: " << *(e.ball())
         << " " << *(e.object());
  }

  void onBallHitBrick(BallHitBrickEventArgs& e) {
    ofLog(_level) << "EVENT: ballHitBrick: " << *(e.ball())
         << " " << e.object();
  }

  void onBallHitWall(BallHitWallEventArgs& e) {
    ofLog(_level) << "EVENT: ballHitWall: " << *(e.ball())
         << " " << *(e.object());
  }
  
  void onBallHitBall(BallHitBallEventArgs& e) {
    ofLog(_level) << "EVENT: ballHitBall: " << *(e.ball())
         << " " << *(e.object());
  }
private:
  ofLogLevel _level;
};


void CollisionEventSource::enableLogging(ofLogLevel level) {
  disableLogging();
  _logger.reset(new CollisionLogger(level));
  attachListener(*_logger);
}

void CollisionEventSource::disableLogging() {
  if (_logger)
    removeListener(*_logger);
  _logger.reset();
}

