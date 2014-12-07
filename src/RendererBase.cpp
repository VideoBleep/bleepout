//
//  RendererBase.cpp
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#include "RendererBase.h"


void RendererBase::draw(RoundState &state) {
  for (int i = 0, count = state.bricks().size(); i < count; i++) {
    drawBrick(state, *(state.bricks()[i].get()));
  }
  for (int i = 0, count = state.paddles().size(); i < count; i++) {
    drawPaddle(state, *(state.paddles()[i].get()));
  }
  for (int i = 0, count = state.balls().size(); i < count; i++) {
    drawBall(state, *(state.balls()[i].get()));
  }
}