//
//  GameEvents.cpp
//  bleepout
//
//  Created by tekt on 12/4/14.
//
//

#include "GameEvents.h"
#include <ofMain.h>


void RoundEventSender::notifyBallHitPaddle(Ball &ball, Paddle &paddle) {
  ofLogVerbose() << "EVENT: ballHitPaddle: " << ball << " " << paddle;
  BallHitPaddleEventArgs e(ball, paddle);
  ofNotifyEvent(ballHitPaddleEvent, e);
}

void RoundEventSender::notifyBallHitBrick(Ball &ball, Brick &brick) {
  ofLogVerbose() << "EVENT: ballHitBrick: " << ball << " " << brick;
  BallHitBrickEventArgs e(ball, brick);
  ofNotifyEvent(ballHitBrickEvent, e);
}
