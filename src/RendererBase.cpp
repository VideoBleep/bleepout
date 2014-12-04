//
//  RendererBase.cpp
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#include "RendererBase.h"


void RendererBase::draw(RoundController &round) {
  for (ofPtr<Brick>& brick : round.bricks()) {
    drawBrick(round, *brick);
  }
  for (ofPtr<Paddle>& paddle : round.paddles()) {
    drawPaddle(round, *paddle);
  }
  for (ofPtr<Ball>& ball : round.balls()) {
    drawBall(round, *ball);
  }
}