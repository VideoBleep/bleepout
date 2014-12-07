//
//  GameEvents.cpp
//  bleepout
//
//  Created by tekt on 12/4/14.
//
//

#include "GameEvents.h"
#include <ofMain.h>


void CollisionEventSource::notifyBallHitPaddle(Ball &ball, Paddle &paddle) {
  BallHitPaddleEventArgs e(ball, paddle);
  ofNotifyEvent(ballHitPaddleEvent, e);
}

void CollisionEventSource::notifyBallHitBrick(Ball &ball, Brick &brick) {
  BallHitBrickEventArgs e(ball, brick);
  ofNotifyEvent(ballHitBrickEvent, e);
}

void CollisionEventSource::notifyBallHitWall(Ball &ball, Wall &wall) {
  BallHitWallEventArgs e(ball, wall);
  ofNotifyEvent(ballHitWallEvent, e);
}

void CollisionEventSource::notifyBallHitBall(Ball &ball, Ball &otherBall) {
  BallHitBallEventArgs e(ball, otherBall);
  ofNotifyEvent(ballHitBallEvent, e);
}

void CollisionLogger::onBallHitPaddle(BallHitPaddleEventArgs &e) {
  ofLogVerbose() << "EVENT: ballHitPaddle: " << e.ball() << " " << e.object();
}

void CollisionLogger::onBallHitBrick(BallHitBrickEventArgs &e) {
  ofLogVerbose() << "EVENT: ballHitBrick: " << e.ball() << " " << e.object();
}

void CollisionLogger::onBallHitWall(BallHitWallEventArgs &e) {
  ofLogVerbose() << "EVENT: ballHitWall: " << e.ball() << " " << e.object();
}

void CollisionLogger::onBallHitBall(BallHitBallEventArgs &e) {
  ofLogVerbose() << "EVENT: ballHitBall: " << e.ball() << " " << e.object();
}

