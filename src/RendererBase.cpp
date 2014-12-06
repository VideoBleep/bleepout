//
//  RendererBase.cpp
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#include "RendererBase.h"


void RendererBase::draw(RoundController &round) {
  for (int i = 0, count = round.bricks().size(); i < count; i++) {
    drawBrick(round, *(round.bricks()[i].get()));
  }
  for (int i = 0, count = round.paddles().size(); i < count; i++) {
    drawPaddle(round, *(round.paddles()[i].get()));
  }
  for (int i = 0, count = round.balls().size(); i < count; i++) {
    drawBall(round, *(round.balls()[i].get()));
  }
}